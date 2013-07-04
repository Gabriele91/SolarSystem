#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>

using namespace SolarSystem;

int main(int argc,const char** args){
	Application::create();
	Table configureTable("SolarSystem.conf");
	Application::instance()->exec(new SolarMain(configureTable));
	delete Application::instance()->getMainInstance();
	Application::close();
	return 0; 
}