#include <stdafx.h>
#include <Texture.h>
#include <Application.h>
#include <Debug.h>
#define IMAGE_LOADER_OPENGL
#include "Image/Image.h"

///////////////////////
using namespace SolarSystem;
///////////////////////
Texture::Texture(const Utility::Path& path)
				:bBilinear(true)
				,chBlr(true)
				,bMipmaps(true)
				,chMps(true)
				,width(0)
				,height(0)
				,gpuid(0){
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(path,data,len);
	//load image
	Image image;
	image.loadFromData(data,
					   len,
					   Image::getTypeFromExtetion(path.getExtension()));
	//free raw file
	free(data);
	//save width end height
	width=image.width;
	height=image.height;
	//create a gpu texture
	buildTexture(image.bytes,image.type);
	//
}
Texture::Texture(const Vec4& floatColor,uint newWidth,uint newheight)
				:bBilinear(true)
				,chBlr(true)
				,bMipmaps(true)
				,chMps(true)
				,width(0)
				,height(0)
				,gpuid(0){
	//build
	Image image(newWidth,newheight,4,true,Image::rgba(&(floatColor.x)));
	//save width end height
	width=newWidth;
	height=newheight;
	//create a gpu texture
	buildTexture(image.bytes,image.type);
}
//send texture to gpu
void Texture::buildTexture(void *data,uint type){
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build
	bind();
	//create a gpu texture
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 type,
				 width,
				 height,
				 0,
				 type,
				 GL_UNSIGNED_BYTE,
				 0);
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
					 width,
					 height,
					 type,
					 GL_UNSIGNED_BYTE,
					 data );
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
}
//destructor
Texture::~Texture(){
	//unload
	DEBUG_ASSERT(gpuid);
    glDeleteTextures(1, &gpuid );
	//reset values
    width = height = 0;
    gpuid = 0;
}
//
void Texture::bind(uint ntexture){
	//
	DEBUG_ASSERT(gpuid);
	//
    glActiveTexture( GL_TEXTURE0 + ntexture );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, (GLuint)gpuid );
	//settings
	if(chBlr){
		chBlr=false;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
	}
	if(chMps){
		chMps=false;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,bMipmaps?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR);
	}
}
void Texture::unbind(uint ntexture){
	glActiveTexture( GL_TEXTURE0 + ntexture );
	glDisable(GL_TEXTURE_2D);
}
//setting
bool Texture::bilinear(){
	return bBilinear;
}
bool Texture::bilinear(bool value){
	chBlr=bBilinear!=value;
	return bBilinear=value;
}
bool Texture::mipmaps(){
	return bMipmaps;
}
bool Texture::mipmaps(bool value){
	chMps=bMipmaps!=value;
	return bMipmaps=value;
}
//operators
bool Texture::operator ==(const Texture& t) const{
	return gpuid==t.gpuid;
}
bool Texture::operator !=(const Texture& t) const{
	return gpuid!=t.gpuid;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
Texture1D::Texture1D(const Utility::Path& path)
					:bBilinear(true)
					,chBlr(true)
					,width(0)
					,gpuid(0){
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(path,data,len);
	//load image
	Image image;
	image.loadFromData(data,
					   len,
					   Image::getTypeFromExtetion(path.getExtension()));
	//free raw file
	free(data);
	//save width end height
	width=image.width;
	//create a gpu texture
	buildTexture1D(image.bytes,image.type);
	//
}
Texture1D::Texture1D(const Vec4& floatColor,uint newWidth)
					:bBilinear(true)
					,chBlr(true)
					,width(0)
					,gpuid(0){
	//build
	Image image(newWidth,1,4,true,Image::rgba(&(floatColor.x)));
	//save width end height
	width=newWidth;
	//create a gpu texture
	buildTexture1D(image.bytes,image.type);
}
//send texture to gpu
void Texture1D::buildTexture1D(void *data,uint type){
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build
	bind();
	//create a gpu texture
	glTexImage1D(GL_TEXTURE_1D,
				 0,
				 type,
				 width,
				 0,
				 type,
				 GL_UNSIGNED_BYTE,
				 0);
	//send to GPU
    glTexSubImage1D( GL_TEXTURE_1D, 0, 0,
					 width,
					 type,
					 GL_UNSIGNED_BYTE,
					 data );
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
}
//destructor
Texture1D::~Texture1D(){
	//unload
	DEBUG_ASSERT(gpuid);
    glDeleteTextures(1, &gpuid );
	//reset values
    width = 0;
    gpuid = 0;
}
//
void Texture1D::bind(uint ntexture){
	//
	DEBUG_ASSERT(gpuid);
	//
    glActiveTexture( GL_TEXTURE0 + ntexture );
	glEnable( GL_TEXTURE_1D );
	glBindTexture( GL_TEXTURE_1D, (GLuint)gpuid );
	//settings
	if(chBlr){
		chBlr=false;
		glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
	}
}
void Texture1D::unbind(uint ntexture){
	glActiveTexture( GL_TEXTURE0 + ntexture );
	glDisable(GL_TEXTURE_1D);
}
//setting
bool Texture1D::bilinear(){
	return bBilinear;
}
bool Texture1D::bilinear(bool value){
	chBlr=bBilinear!=value;
	return bBilinear=value;
}
//operators
bool Texture1D::operator ==(const Texture1D& t) const{
	return gpuid==t.gpuid;
}
bool Texture1D::operator !=(const Texture1D& t) const{
	return gpuid!=t.gpuid;
}
//////////////////////////////////////////////

//////////////////////////////////////////////
//render texture:
//costructor
RenderTexture::RenderTexture(uint argwidth,uint argheight):Texture(),fboid(0),depthid(0){
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//not work midmaps
	bMipmaps=false;
	//build
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, (GLuint)gpuid );
	//save width end height
	width=argwidth;
	height=argheight;
	//create a gpu texture
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 width,
				 height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 0);
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );
	//unbind texture
	glBindTexture( GL_TEXTURE_2D, 0 );
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
	//////////////////////////////////
	//df buffer
	glGenRenderbuffersEXT(1, &depthid);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthid);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
							 GL_DEPTH_COMPONENT24,
							 width,
							 height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
	//////////////////////////////////
	//create VBO
	glGenFramebuffersEXT(1, &fboid);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboid);
	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
							  GL_COLOR_ATTACHMENT0_EXT ,
							  GL_TEXTURE_2D,
							  gpuid,
							  0);
    //-------------------------
    //Attach depth buffer to FBO
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
								 GL_DEPTH_ATTACHMENT_EXT,
								 GL_RENDERBUFFER_EXT,
								 depthid);
	//set draw buffer
	//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	//glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	//get errors...
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
		DEBUG_MESSAGE("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	}
	// disabilita fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
}
//destructor
RenderTexture::~RenderTexture(){

	//unload
	DEBUG_ASSERT(gpuid);
    glDeleteRenderbuffersEXT(1, &depthid);
	glDeleteFramebuffersEXT(1, &fboid);
}
//start draw
void RenderTexture::enableRender(){
	//abilita fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboid);
}
//end draw
void RenderTexture::disableRender(){
	//disabilita fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
}
//draw in fullScreen
void RenderTexture::draw(bool bindTexture){
	//reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//reset model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//
	static const float
	size=1.0;
	static const float
	xyUV[]={
			-size,-size,0.0,0.0,
			-size, size,0.0,1.0,
			 size,-size,1.0,0.0,
			 size, size,1.0,1.0
	        };

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	//set texture
	if(bindTexture) bind();
	//set vertex
	glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
	//draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//disable texture
	if(bindTexture) unbind();
}
//////////////////////////////////////////////


//////////////////////////////////////////////
//render texture:
//costructor
ShadowTexture::ShadowTexture(uint argwidth,uint argheight):Texture(),fboid(0){
	//not work midmaps
	bMipmaps=false;		
	//save width end height
	width=argwidth;
	height=argheight;
	// Try to use a texture depth component
	glGenTextures(1, &gpuid);
	glBindTexture(GL_TEXTURE_2D, gpuid);	
	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF. Using GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );	
	// This is to allow usage of shadow2DProj function in the shader
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glTexImage2D( GL_TEXTURE_2D, 
		          0, 
		          GL_DEPTH_COMPONENT, 
				  width, 
				  height, 
				  0, 
				  GL_DEPTH_COMPONENT, 
				  GL_UNSIGNED_BYTE, 0);

	glBindTexture( GL_TEXTURE_2D, 0 ); 
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
	//////////////////////////////////
	//create VBO
	glGenFramebuffersEXT(1, &fboid);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboid);	
	// Instruct openGL that we won't bind a color texture with the currently binded FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// Instruct openGL that we won't bind a depth texture with the currently binded FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
							  GL_DEPTH_ATTACHMENT_EXT ,
							  GL_TEXTURE_2D,
							  gpuid,
							  0);
	//get errors...
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT){
		DEBUG_MESSAGE("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
	}
	// disabilita fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
}
//destructor
ShadowTexture::~ShadowTexture(){

	//unload
	DEBUG_ASSERT(gpuid);
	glDeleteFramebuffersEXT(1, &fboid);
}
//start draw
void ShadowTexture::enableRender(){
	//abilita fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboid);
}
//end draw
void ShadowTexture::disableRender(){
	//disabilita fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
}
//draw in fullScreen
void ShadowTexture::draw(bool bindTexture){
	//reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//reset model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//
	static const float
	size=1.0;
	static const float
	xyUV[]={
			-size,-size,0.0,0.0,
			-size, size,0.0,1.0,
			 size,-size,1.0,0.0,
			 size, size,1.0,1.0
	        };

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	//set texture
	if(bindTexture) bind();
	//set vertex
	glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
	//draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//disable texture
	if(bindTexture) unbind();
}
//////////////////////////////////////////////

