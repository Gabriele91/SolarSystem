#include <stdafx.h>
#include <PlanetsManager.h>
#include <Application.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
PlanetsManager::PlanetsManager(const Utility::Path& path,
							   Camera *camera,
							   SolarRender *render)
	:sun(0),camera(camera),render(render),configfile(path)
	,blackMesh("shader/blackMesh.vs","shader/blackMesh.ps")
	,godRays("shader/godRays.vs","shader/godRays.ps")
	,sunLight("shader/sunLight.vs","shader/sunLight.ps")
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
	//setup config file:
	DEBUG_ASSERT_MSG(configfile.existsAsType("sun",Table::TABLE),"PlanetsManager error : not found sun table");
	DEBUG_ASSERT_MSG(configfile.existsAsType("planets",Table::TABLE),"PlanetsManager error : not found sun planets");
	//gat tables
	const Table& sun=configfile.getTable("sun");
	const Table& planets=configfile.getTable("planets");
	//set scale
	setScaleEllipses(configfile.getFloat("scaleEllipses",0.07));
	setScalePlanets(configfile.getFloat("scalePlanets",0.15));
	setScaleSun(configfile.getFloat("scaleSun",1.5));
	//add sun
	addSun(sun.getString("image"),sun.getVector3D("scale"),sun.getFloat("period"));
	//add planets:
	for(auto& itTable:planets){
		const Table& planet=itTable.second->get<Table>();
		if(planet.existsAsType("cloud",Table::STRING))
			addPlanet(planet.getString("image"),
					  planet.getString("cloud"),
					  planet.getVector2D("ellipse"),
					  planet.getVector3D("scale"),
					  planet.getFloat("daysInYear"),
					  planet.getFloat("period"));
		else
			addPlanet(planet.getString("image"),
					  planet.getVector2D("ellipse"),
					  planet.getVector3D("scale"),
					  planet.getFloat("daysInYear"),
					  planet.getFloat("period"));
	}

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
							   const Vec2& ellipse,
							   const Vec3& scale,
							   float daysInYear,
							   float rotationPeriod){
	Planet* planet=new Planet(path);
	planet->setPlanetInfo(ellipse*scaleEllipses,daysInYear,rotationPeriod);
	planet->setScale(scale*scalePlanets);
	planets.push_back(planet);
}
void PlanetsManager::addPlanet(const Utility::Path &path,
							   const Utility::Path &coudPath,
							   const Vec2& ellipse,
							   const Vec3& scale,
							   float daysInYear,
							   float rotationPeriod){
	Planet* planet=new Planet(path,coudPath);
	planet->setPlanetInfo(ellipse*scaleEllipses,daysInYear,rotationPeriod);
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
	////////////////////////////////////////////////////////////////////
	//offscreen draw
	blackTexture.enableRender();
		//set viewport
		glViewport(0, 0, blackTexture.getWidth(),
						 blackTexture.getHeight());
		//clear
		render->setClearColor(Vec4(0.0f, 0.0f, 0.0f, 0.0f));
		//
		drawSun();
		blackMesh.bind();
			drawPlanets();
		blackMesh.unbind();
		//
	blackTexture.disableRender();
	////////////////////////////////////////////////////////////////////
	//reset viewport
	glViewport(oldViewport.x,oldViewport.y,oldViewport.z,oldViewport.w);
	
	//draw word
	drawSun();
	//load view matrix
	glLoadMatrixf(camera->getGlobalMatrix());
	//set lights
	render->setLight(sun->getPosition(),
					 Vec4(1.0,1.0,1.0,1.0),
					 Vec4(1.0,1.0,1.0,1.0),
					 Vec4(1.0,1.0,1.0,1.0));
	 GLfloat Kc = 0.01;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
	render->setMaterial(Vec4(0.0,0.0,0.0,1.0),
					    Vec4(1.0,1.0,1.0,1.0),
					    Vec4(0.6,0.6,0.6,1.0),
					    Vec4(0.1,0.1,0.1,1.0),
						15.0f);
	render->enableLight();
	sunLight.bind();
	sunLight.uniformInt("planetTexture",0);
		drawPlanets();
	sunLight.unbind();
	render->disableLight();

	//draw vbo
	godRays.bind();
	float uniformExposure = 0.0044f;
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