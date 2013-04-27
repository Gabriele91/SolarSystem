#include <stdafx.h>
#include <PlanetsManager.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
PlanetsManager::PlanetsManager(Camera *camera):camera(camera){}
PlanetsManager::~PlanetsManager(){	
	for(auto planet:planets) delete planet;
}

void PlanetsManager::addSun(const Utility::Path &path,const Vec3& scale){	
	Planet* planet=new Planet(path);
	planet->setPlanetInfo(Vec2::ZERO,1);
	planet->setScale(scale*scaleSun);
	planets.push_back(planet);
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
	for(auto planet:planets)
		planet->draw(*camera);
}