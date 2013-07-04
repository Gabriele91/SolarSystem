#include <stdafx.h>
#include <Application.h>
#include <Screen.h>
#include <Input.h>
#include <MainInstance.h>
///////////////////////
using namespace SolarSystem;

MainInstance::MainInstance(const String& app,
						   unsigned int width,
						   unsigned int height,
						   unsigned int bites,
						   unsigned int freamPerSecond,
						   bool fullscreen,
						   int dfAA){
Application::instance()
			 ->getScreen() 
			 ->createWindow(app.c_str(),width,height,bites,freamPerSecond,fullscreen,(Screen::AntiAliasing)dfAA);
}

Screen* MainInstance::getScreen(){
	return Application::instance()->getScreen();
}

Audio* MainInstance::getAudio(){
	return Application::instance()->getAudio();
}

Input* MainInstance::getInput(){
	return Application::instance()->getInput();
}