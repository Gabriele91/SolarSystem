#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>
#include <SolarStartMenu.h>
#include <APIAudio.h>
#include <Sound.h>

using namespace SolarSystem;

int main(int argc,const char** args){
	//open audio device
	APIAudio::init();
	Sound *music=NULL;

	//open an application instance
	Application::create();

	//load table
	Table configureTable("SolarSystem.conf.e2d");
	//load sound
	if(configureTable.existsAsType("sound",Table::STRING)) {
		music=new Sound(configureTable.getString("sound"));
		music->loop();
	}

	//start menu
	SolarStartMenu *solarmenu=new SolarStartMenu(configureTable);
	Application::instance()->exec(solarmenu);
	bool isNotClose=!solarmenu->doCloseApp();
	delete solarmenu;
	Application::close();

	//start application solar
	if(isNotClose){		
		Application::create();
		SolarMain *solarmain=new SolarMain(configureTable);
		Application::instance()->exec(solarmain);
		delete solarmain;
		Application::close();
	}
	
	//close audio device
	if(music) delete music;
	APIAudio::exit();
	return 0; 
}