//
//  CocoaScreen.mm
//  Easy2D
//
//  Created by Gabriele Di Bari on 14/06/13.
//
//

#include <CocoaScreen.h>
#include <Cocoa/Cocoa.h>
#import <AppKit/NSOpenGLView.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSScreen.h>

using namespace SolarSystem;
#define TOCOCOAWINDOW NSWindow * window=(NSWindow *)(cocoaWindow);
#define TOCOCOACONTEXT NSOpenGLContext * openGLContext=(NSOpenGLContext *)(cocoaGLContext);


///////////////////////////////////////////////////////////////////////////////
// WINDOW CLASS
@interface  Easy2DWindow : NSWindow
//! These are needed for borderless/fullscreen windows 
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
//! In addition to closing the window, also terminate the app.
- (void) close;
//! @return yes
- (BOOL) acceptsFirstResponder;
@end

@implementation Easy2DWindow
- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}

- (BOOL) acceptsFirstResponder {
    return YES;
}

- (void) close {
    [NSApp terminate:self];    
    // If the app refused to terminate, this window should still close.
    [super close];
}

@end
///////////////////////////////////////////////////////////////////////////////
// VIEW CLASS
@interface Easy2DView : NSView
// The default implementation doesn't pass rightMouseDown to responder chain
- (void)rightMouseDown:(NSEvent *)theEvent;
@end

@implementation Easy2DView
- (void)rightMouseDown:(NSEvent *)theEvent
{
	[[self nextResponder] rightMouseDown:theEvent];
}
@end
///////////////////////////////////////////////////////////////////////////////


void CocoaScreen::__openWindow(int w,int h,const char *title){
    
    //GC
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    //size window
    NSRect frame;
    frame.origin.x = 0;
    frame.origin.y = 0;
    frame.size.width = w;
    frame.size.height = h;
	
    //window
    NSWindow * window = [[Easy2DWindow alloc]
                          initWithContentRect: frame
                          styleMask: (NSTitledWindowMask | NSMiniaturizableWindowMask | NSClosableWindowMask)
                          backing: NSBackingStoreBuffered
                          defer: false];    
    //  Set view
	NSView *contentView = [[Easy2DView alloc] initWithFrame:frame];
	[window setContentView: contentView];
    [window makeFirstResponder:contentView];
	[contentView release];
    
    //set title
	[window setReleasedWhenClosed:false];
    NSString *titleNS = [NSString stringWithUTF8String:title];
    [window setTitle:titleNS];
	[window center];
	[window makeKeyAndOrderFront:nil];
    
    //GC release
	[pool release];
    
    //set screen size
    selectFullScreenWidth=screenWidth=w;
    selectFullScreenHeight=screenHeight=h;
    
    //save object
    cocoaWindow=(void*)window;
    
}
void CocoaScreen::__closeWindow(){
    
    TOCOCOAWINDOW
    [window close];
    [window release];
    

}
void CocoaScreen::__createContext(int msaa){
    //COCOA OPENGL CONTEXT
    NSOpenGLContext *openGLContext=NULL;
    
    NSOpenGLPixelFormatAttribute attributes[32];    
    int i=0;
    attributes[i++] = NSOpenGLPFANoRecovery;
    //attributes[i++] = NSOpenGLPFAAccelerated;
    attributes[i++] = NSOpenGLPFADoubleBuffer;
    attributes[i++] = NSOpenGLPFAColorSize;
    attributes[i++] = 32.0;
    attributes[i++] = NSOpenGLPFADepthSize;
    attributes[i++] = 32.0;
    //msaa
    if(msaa!=NOAA){
        attributes[i++]=NSOpenGLPFASampleBuffers;
        attributes[i++]=1;
        attributes[i++]=NSOpenGLPFASamples;
        attributes[i++]=static_cast<NSOpenGLPixelFormatAttribute>(msaa);
    }
    
    attributes[i] = 0;
    
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    DEBUG_ASSERT(pixelFormat);

    openGLContext = [[NSOpenGLContext alloc]  initWithFormat:pixelFormat shareContext:nil];
    DEBUG_ASSERT(openGLContext);
    
    //swap interval    
    GLint value = 1;
    [openGLContext setValues:&value forParameter:NSOpenGLCPSwapInterval];
    
    //dealloc
    [pixelFormat release];
    /////////////////////////
    //return
    cocoaGLContext=(void*)openGLContext;


}
void CocoaScreen::__deleteContext(){
    TOCOCOACONTEXT
    [NSOpenGLContext clearCurrentContext];
    [openGLContext clearDrawable];
    [openGLContext release];
}

void CocoaScreen::__initOpenGL(){
    //enable culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    //default status for blending
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc( GL_ONE , GL_ZERO );
    //disable zbuffer
    glDisable(GL_DEPTH_TEST);
    //disable light
    glDisable(GL_LIGHTING);
    //enable texturing
    glEnable( GL_TEXTURE_2D );
    //always active!
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    //find errors:
    CHECK_GPU_ERRORS();
    
}
/**
 * cocoa screen
 */
CocoaScreen::CocoaScreen()
:nativeWidth(0)
,nativeHeight(0)
,screenWidth(0)
,screenHeight(0)
,freamPerSecond(0)
,fullscreen(false)
,showmouse(true)
,onCocoaWindowCreated(NULL)
{
    NSRect screenRect;
    NSArray *screens=[NSScreen screens];
    long cntScreens=[screens count];
    for(long i=0;i<cntScreens;++i){
        screenRect=[[screens objectAtIndex:i] frame];
    }
    //screen size
    nativeWidth=screenRect.origin.x+screenRect.size.width;
    nativeHeight=screenRect.origin.y+screenRect.size.height;
}
CocoaScreen::~CocoaScreen(){
    __deleteContext();
    __closeWindow();
}

/**
 * return screen width
 */
uint CocoaScreen::getWidth(){
	return screenWidth;
}
/**
 * return screen height
 */
uint CocoaScreen::getHeight(){
	return screenHeight;
}
/**
 * return screen native width
 */
uint CocoaScreen::getNativeWidth(){
	return nativeWidth;
}
/**
 * return screen  native height
 */
uint CocoaScreen::getNativeHeight(){
	return nativeHeight;
}
/**
 * return screen orientation
 */
Screen::Orientation CocoaScreen::getOrientation(){
	return Screen::LANDSCAPE_LEFT;
}
/**
 * return frame rate
 */
uint CocoaScreen::getFrameRate(){
	return freamPerSecond;    
}

/**
 * set the specified thread's current rendering context
 */
void CocoaScreen::acquireContext(){
    
    TOCOCOACONTEXT
    
	[openGLContext makeCurrentContext];
    
}
/**
 * swap the buffers
 */
void CocoaScreen::swap(){
    
    //TOCOCOACONTEXT
    glSwapAPPLE();
	//[openGLContext flushBuffer];
    
}
/**
 * show or hide mouse cursor
 * @param show, set true if you want show cursor otherwise false
 */
void CocoaScreen::setCursor(bool show){
    if (show) CGDisplayShowCursor(kCGDirectMainDisplay);
    else CGDisplayHideCursor(kCGDirectMainDisplay);
}
/**
 * set position cursor
 */
void CocoaScreen::setPositionCursor(const Vec2& pos){
    
    TOCOCOAWINDOW
    NSView* view=window.contentView;
    //calc inverse top view
    float screenH=window.screen.frame.origin.y+window.screen.frame.size.height;
    float windowH=window.frame.origin.y+window.frame.size.height;
    float viewTop=window.frame.size.height-view.frame.size.height;
    //calc mouse point in screen
    CGPoint cgpoint;
    cgpoint.x =   window.frame.origin.x + Math::max(Math::min(pos.x,getWidth()*1.0f),0.0f);
    cgpoint.y =   (screenH-windowH+viewTop) + Math::max(Math::min(pos.y,getHeight()*1.0f),1.0f);
    //send event
    CGEventRef event = CGEventCreateMouseEvent(NULL,
                                               kCGEventMouseMoved,
                                               cgpoint,
                                               /*we don't care about this : */0);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
    
}
/**
 * return if cursor is shown or hidden
 */
bool CocoaScreen::getCursor(){
	return CGCursorIsVisible();;
}
/**
 * enable or disable full screen
 */
void CocoaScreen::setFullscreen(bool fullscreen){
    
    TOCOCOAWINDOW
    //this
    this->fullscreen=fullscreen;
    //get view
    NSView* view=window.contentView;
    //enable fullscreen
    if(fullscreen){
        [view enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
        [[view window] setAcceptsMouseMovedEvents:YES];
        //change size screen
        screenWidth=nativeWidth;
        screenHeight=nativeHeight;
    }
    else{
        [view exitFullScreenModeWithOptions:nil];
        //change size screen
        screenWidth=selectFullScreenWidth;
        screenHeight=selectFullScreenHeight;
    }
}
/**
 * return if fullscreen is enable return true
 */
bool CocoaScreen::isFullscreen(){
	return fullscreen;
}

/**
 * create window
 */
void CocoaScreen::createWindow(const char* appname,
                          uint width,
                          uint height,
                          uint bites,
                          uint freamPerSecond,
                          bool fullscreen,
                          AntiAliasing dfAA){
    this->fullscreen=fullscreen;
    this->cocoaGLContext=NULL;
    this->cocoaWindow=NULL;
    this->freamPerSecond=freamPerSecond;
    //open window
    __openWindow(width,height,appname);
    //create context
    __createContext(dfAA);
    //set context to window   
    TOCOCOAWINDOW
    TOCOCOACONTEXT
    [openGLContext setView:[window contentView]];
	[openGLContext update];
    [[window contentView] display];
    //set context
    acquireContext();    
    //init openGL
    __initOpenGL();
    //fullscreen
    setFullscreen(fullscreen);
    //enable AA
    if(dfAA!=NOAA)
        glEnable( GL_MULTISAMPLE );
    //add listener
    if(onCocoaWindowCreated)
        onCocoaWindowCreated(cocoaWindow);
}
/*
 * close window
 */
void CocoaScreen::closeWindow(){
    __closeWindow();    
}