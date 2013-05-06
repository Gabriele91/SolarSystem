#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>

using namespace SolarSystem;

int main(int argc,const char** args){
	String configfile("SolarSystem.conf");
	if(argc>=2)	configfile=args[1];
	Application::create();
	Application::instance()->exec(new SolarMain(configfile));
	delete Application::instance()->getMainInstance();
	return 0; 
}