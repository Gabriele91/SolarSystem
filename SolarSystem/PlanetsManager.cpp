#include <stdafx.h>
#include <PlanetsManager.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
PlanetsManager::PlanetsManager(Camera *camera):sun(0),camera(camera){}
PlanetsManager::~PlanetsManager(){	
	for(auto planet:planets) delete planet;
	if(sun) delete sun;
}

void PlanetsManager::addSun(const Utility::Path &path,const Vec3& scale){	
	sun=new Planet(path);
	sun->setPlanetInfo(Vec2::ZERO,1);
	sun->setScale(scale*scaleSun);
}

void PlanetsManager::addPlanet(const Utility::Path &path,
							   const Vec2& elipse,
							   const Vec3& scale,
							   float daysInYear){
	Planet* planet=new Planet(path);
	planet->setPlanetInfo(elipse*scaleElipses,daysInYear);
	planet->setScale(scale*scalePlanets);
	planets.push_back(planet);
}

void PlanetsManager::setData(float day){
	for(auto planet:planets)
		planet->setData(day);
}
void PlanetsManager::draw(){
	camera->update();
	drawSun();
	drawPlanets();
}
void PlanetsManager::drawPlanets(){
	for(auto planet:planets)
		planet->draw(*camera);
}
void PlanetsManager::drawSun(){
	sun->draw(*camera);
}