#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>
#include <SolarStartMenu.h>
#include <SolarSound.h>

using namespace SolarSystem;

int main(int argc,const char** args){
	//open audio device
	APIAudio::init();

	//open an application instance
	Application::create();

	//load table
	Table configureTable("SolarSystem.conf.e2d");
	SolarSound *solarMusic=new SolarSound(configureTable);
	//start menu
	solarMusic->loopMenu();
	SolarStartMenu *solarmenu=new SolarStartMenu(configureTable);
	Application::instance()->exec(solarmenu);
	bool isNotClose=!solarmenu->doCloseApp();
	delete solarmenu;
	Application::close();

	//start application solar
	if(isNotClose){			
		solarMusic->loopSystem();	
		Application::create();
		SolarMain *solarmain=new SolarMain(configureTable);
		Application::instance()->exec(solarmain);
		delete solarmain;
		Application::close();
	}
	
	//close audio device
	delete solarMusic;
	APIAudio::exit();
	return 0; 
}