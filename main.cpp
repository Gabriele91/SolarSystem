#include <stdafx.h>
#include <Application.h>
#include <SolarMain.h>

using namespace SolarSystem;

int main(){
	Application::create();
	Application::instance()->exec(new SolarMain());
	delete Application::instance()->getMainInstance();
	return 0; 
}