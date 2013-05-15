#include <stdafx.h>
#include <Application.h>
#include <ETime.h>
#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )
#elif defined( PLATFORM_WINDOW )
#include <WindowsApp.h>
#elif defined( PLATFORM_LINUX )
#include <LinuxApp.h>
#elif defined( PLATFORM_ANDROID )
#endif
///////////////////////
using namespace SolarSystem;
///////////////////////
Application *Application::appSingleton=NULL;
///////////////////////
Application::Application()
	:mainInstance(NULL)
	,screen(NULL)
	,input(NULL)
	,audio(NULL){

}
Application::~Application(){
	appSingleton=NULL;	
}

Application *Application::create(){

	DEBUG_ASSERT(!appSingleton);
	
	Math::seedRandom((uint)GetTime());

#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )
#elif defined( PLATFORM_WINDOW )
	appSingleton=new WindowsApp();
#elif defined( PLATFORM_LINUX )
	appSingleton=new LinuxApp();
#elif defined( PLATFORM_ANDROID )
#endif
	return appSingleton;
}

Application *Application::instance(){
	return appSingleton;
}

void Application::close(){	
	DEBUG_ASSERT(appSingleton);
	//exit to instance
	::Application::appSingleton->exit();
	//delete instance
	delete ::Application::appSingleton; 
}
