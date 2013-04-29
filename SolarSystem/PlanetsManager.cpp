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
	//get tables
	const Table& sun=configfile.getTable("sun");
	const Table& planets=configfile.getTable("planets");
	//get default material
	Vec4 ambienMat(Vec3::ZERO,1.0f);
	Vec4 diffuseMat(Vec3::ZERO,1.0f);
	Vec4 specularMat(Vec3::ZERO,1.0f);
	Vec4 emissionMat(Vec3::ZERO,1.0f);
	float shininessMat=1.0;
	if(configfile.existsAsType("material",Table::TABLE)){
		const Table& material=configfile.getTable("material");
		ambienMat=material.getVector4D("ambien",ambienMat);
		diffuseMat=material.getVector4D("diffuse",diffuseMat);
		specularMat=material.getVector4D("specular",specularMat);
		emissionMat=material.getVector4D("emission",emissionMat);
		shininessMat=material.getFloat("shininess",shininessMat);
	}
	//set scale
	setScaleEllipses(configfile.getFloat("scaleEllipses",0.07));
	setScalePlanets(configfile.getFloat("scalePlanets",0.15));
	setScaleSun(configfile.getFloat("scaleSun",1.5));
	//add sun
	addSun(sun.getString("image"),sun.getVector3D("scale"),sun.getFloat("period"));
	//add planets:
	for(auto& itTable:planets){
		
		Vec4 ambien(ambienMat);
		Vec4 diffuse(diffuseMat);
		Vec4 specular(specularMat);
		Vec4 emission(emissionMat);
		float shininess=shininessMat;

		const Table& planet=itTable.second->get<Table>();
		if(planet.existsAsType("material",Table::TABLE)){
			const Table& material=planet.getConstTable("material");
			ambien=material.getVector4D("ambien",ambienMat);
		    diffuse=material.getVector4D("diffuse",diffuseMat);
		    specular=material.getVector4D("specular",specularMat);
		    emission=material.getVector4D("emission",emissionMat);
			shininess=material.getFloat("shininess",shininessMat);
		}
		Planet *ptr=
		addPlanet(planet.getString("image"),
					planet.getVector2D("ellipse"),
					planet.getVector3D("scale"),
					planet.getFloat("daysInYear"),
					planet.getFloat("period"),
					ambien,
					diffuse,
					specular,
					emission,
					shininess);
		
		if(planet.existsAsType("cloud",Table::STRING))
			ptr->setCloudTexture(planet.getString("cloud"));
		if(planet.existsAsType("night",Table::STRING))
			ptr->setBlackTexture(planet.getString("night"));
		
	}

}
PlanetsManager::~PlanetsManager(){	
	for(auto planet:planets) delete planet;
	if(sun) delete sun;
}

void PlanetsManager::addSun(const Utility::Path &path,
							const Vec3& scale,
							float rotationPeriod){	
	sun=new Planet(render,path);
	sun->setPlanetInfo(Vec2::ZERO,1,rotationPeriod);
	sun->setScale(scale*scaleSun);
}

Planet* PlanetsManager::addPlanet( //texture
								   const Utility::Path &path,
								   //physics
								   const Vec2& ellipse,
								   const Vec3& scale,
								   float daysInYear,
								   float rotationPeriod,
								   //material
								   const Vec4& ambient,
								   const Vec4& diffuse,
								   const Vec4& specular,
								   const Vec4& emission,
								   float shininess){
	Planet* planet=new Planet(render,path);
	planet->setPlanetInfo(ellipse*scaleEllipses,daysInYear,rotationPeriod);
	planet->setMaterial(ambient,diffuse,specular,emission,shininess);
	planet->setScale(scale*scalePlanets);
	planets.push_back(planet);
	return planet;
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
	auto blendState=render->getBlendState();
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
	 GLfloat Kc = 0.01f;
     GLfloat Kl = 0.0f;
     GLfloat Kq = 0.0f;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);

	render->enableLight();
	sunLight.bind();
	sunLight.uniformInt("planetTexture",0);
	sunLight.uniformInt("planetNightTexture",1);
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
	glEnable( GL_BLEND );   
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	//
	blackTexture.draw();
	godRays.unbind(); 
	//reset old blend state   
	render->setBlendState(blendState);
}
void PlanetsManager::drawPlanets(){
	for(auto planet:planets)
		planet->draw(*camera);
}
void PlanetsManager::drawSun(){
	sun->draw(*camera);
}