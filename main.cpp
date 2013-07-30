#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>
#include <SolarStartMenu.h>

using namespace SolarSystem;

int main(int argc,const char** args){

	Application::create();
	Table configureTable("SolarSystem.conf.e2d");
	
	SolarStartMenu *solarmenu=new SolarStartMenu(configureTable);
	Application::instance()->exec(solarmenu);
	bool isNotClose=!solarmenu->doCloseApp();
	delete solarmenu;
	Application::close();

	if(isNotClose){		
		Application::create();
		SolarMain *solarmain=new SolarMain(configureTable);
		Application::instance()->exec(solarmain);
		delete solarmain;
		Application::close();
	}
	return 0; 
}