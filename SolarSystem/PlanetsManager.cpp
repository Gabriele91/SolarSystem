#include <stdafx.h>
#include <PlanetsManager.h>
#include <Application.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
PlanetsManager::PlanetsManager(Camera *camera)
	:sun(0),camera(camera)
	,blackMesh("shader/blackMesh.vs","shader/blackMesh.ps")
	,godRays("shader/godRays.vs","shader/godRays.ps")
	,blackTexture(Application::instance()->getScreen()->getWidth(),
				  Application::instance()->getScreen()->getHeight())
{
	//get uniform
	glslExposure=godRays.getUniformID("exposure");
	glslDecay=godRays.getUniformID("decay");
	glslDensity=godRays.getUniformID("density");
	glslWeight=godRays.getUniformID("weight");
	glslLightPositionOnScreen=godRays.getUniformID("lightPositionOnScreen");
	glslScreenTexture=godRays.getUniformID("screenTexture");
}
PlanetsManager::~PlanetsManager(){	
	for(auto planet:planets) delete planet;
	if(sun) delete sun;
}

void PlanetsManager::addSun(const Utility::Path &path,
							const Vec3& scale,
							float rotationPeriod){	
	sun=new Planet(path);
	sun->setPlanetInfo(Vec2::ZERO,1,rotationPeriod);
	sun->setScale(scale*scaleSun);
}

void PlanetsManager::addPlanet(const Utility::Path &path,
							   const Vec2& elipse,
							   const Vec3& scale,
							   float daysInYear,
							   float rotationPeriod){
	Planet* planet=new Planet(path);
	planet->setPlanetInfo(elipse*scaleElipses,daysInYear,rotationPeriod);
	planet->setScale(scale*scalePlanets);
	planets.push_back(planet);
}

void PlanetsManager::setData(float day){
	for(auto planet:planets)
		planet->setData(day);
	if(sun) sun->setData(day);
}
void PlanetsManager::draw(){
	camera->update();
	
	//save viewport
	Vec4 oldViewport;
	glGetFloatv(GL_VIEWPORT,&oldViewport.x);
	//save blend
	int SRC_BLEND;
	int DST_BLEND;
	glGetIntegerv(GL_BLEND_SRC_RGB , &SRC_BLEND);
	glGetIntegerv(GL_BLEND_DST_RGB , &DST_BLEND);
	bool BLEND_IS_ENABLE;
	BLEND_IS_ENABLE=glIsEnabled(GL_BLEND);
	//offscreen draw
	blackTexture.enableRender();				
		glViewport(0, 0, blackTexture.getWidth(),
						 blackTexture.getHeight());
		//clear
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		drawSun();
		blackMesh.bind();
			drawPlanets();
		blackMesh.unbind();
	blackTexture.disableRender();
	//reset viewport
	glViewport(oldViewport.x,oldViewport.y,oldViewport.z,oldViewport.w);
	//draw
	drawSun();
	drawPlanets();
	//draw vbo
	godRays.bind();
	float uniformExposure = 0.0034f;
	float uniformDecay = 1.0f;
	float uniformDensity = 1.f;
	float uniformWeight = 3.65f;
	godRays.uniformFloat(glslExposure,uniformExposure);
	godRays.uniformFloat(glslDecay,uniformDecay);
	godRays.uniformFloat(glslDensity,uniformDensity);
	godRays.uniformFloat(glslWeight,uniformWeight);
	godRays.uniformVector2D(glslLightPositionOnScreen,
							camera->getPointIn3DSpace(sun->getPosition()));
	godRays.uniformInt(glslScreenTexture,0);	
	//additive blend
	if(!BLEND_IS_ENABLE) glEnable( GL_BLEND );   
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	//
	blackTexture.draw();
	godRays.unbind(); 
	//reset old blend state   
	if(!BLEND_IS_ENABLE) glDisable( GL_BLEND );   
	glBlendFunc( SRC_BLEND, DST_BLEND );
}
void PlanetsManager::drawPlanets(){
	for(auto planet:planets)
		planet->draw(*camera);
}
void PlanetsManager::drawSun(){
	sun->draw(*camera);
}