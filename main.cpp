#include <stdafx.h>
#include <Application.h>
#include <SolarApp.h>

using namespace SolarSystem;

int main(int argc,const char** args){
	//open audio device
	APIAudio::init();
	//open an application instance
	Application::create();
	//load table
	Table configureTable("SolarSystem.conf.e2d");
    //load app
	SolarApp *solarapp=new SolarApp(configureTable);
    //execute app
	Application::instance()->exec((MainInstance*)solarapp);
    //delete instance
    delete Application::instance()->getMainInstance();
    //close app
	Application::close();
    //clode audio device
	APIAudio::exit();
	return 0; 
}