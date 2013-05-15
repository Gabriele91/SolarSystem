#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>

using namespace SolarSystem;

int main(int argc,const char** args){
	String configfile(argc>=2?args[1]:"SolarSystem.conf");
	Application::create();
	Application::instance()->exec(new SolarMain(configfile));
	delete Application::instance()->getMainInstance();
	Application::close();
	return 0; 
}