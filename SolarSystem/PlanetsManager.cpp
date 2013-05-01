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
	,blackTexture(Application::instance()->getScreen()->getWidth(),
				  Application::instance()->getScreen()->getHeight())
{
	//black shader
	blackMesh.shader.loadShader("shader/blackMesh.vs","shader/blackMesh.ps");
	//load shader sun light (planets):
	sunLight.shader.loadShader("shader/sunLight.vs","shader/sunLight.ps");
	sunLight.glPlanetTexture=sunLight.shader.getUniformID("planetTexture");
	sunLight.glPlanetNightTexture=sunLight.shader.getUniformID("planetNightTexture");
	//load shader sun light (clouds):
	sunLightCloud.shader.loadShader("shader/sunLightClouds.vs","shader/sunLightClouds.ps");
	sunLightCloud.glCloudTexture=sunLightCloud.shader.getUniformID("cloudTexture");
	//load shader sun light (atmospheres):
	sunLightAtmosphere.shader.loadShader("shader/sunLightAtmosphere.vs","shader/sunLightAtmosphere.ps");
	sunLightAtmosphere.glAtmGrad1=sunLightAtmosphere.shader.getUniformID("atmGrad1");
	sunLightAtmosphere.glAtmGrad2=sunLightAtmosphere.shader.getUniformID("atmGrad2");	
	sunLightAtmosphere.atmRim=sunLightAtmosphere.shader.getUniformID("atmRim");	
	//setup config file:
	DEBUG_ASSERT_MSG(configfile.existsAsType("sun",Table::TABLE),"PlanetsManager error : not found sun table");
	DEBUG_ASSERT_MSG(configfile.existsAsType("planets",Table::TABLE),"PlanetsManager error : not found planets table");
	//get tables
	const Table& sun=configfile.getTable("sun");
	const Table& planets=configfile.getTable("planets");
	//get shader info:
	String semples("NUM_SAMPLES 100");	
	godRays.uniformExposure = 0.0044f;
	godRays.uniformDecay = 1.0f;
	godRays.uniformDensity = 1.f;
	godRays.uniformWeight = 3.65f;
	if(sun.existsAsType("shader",Table::TABLE)){
		const Table& shader=sun.getConstTable("shader");
		semples=("NUM_SAMPLES "+String::toString((int)shader.getFloat("semples",100)));		
		godRays.uniformExposure =shader.getFloat("exposure",godRays.uniformExposure);
		godRays.uniformDecay =shader.getFloat("decady",godRays.uniformDecay);
		godRays.uniformDensity =shader.getFloat("density",godRays.uniformDensity);
		godRays.uniformWeight = shader.getFloat("weight",godRays.uniformWeight);
	}
	//////////////////////////////load shader//////////////////////////////
	const char* defines[]={semples,NULL};
	godRays.shader.loadShader("shader/godRays.vs","shader/godRays.ps",defines);
	//get uniform
	godRays.glslExposure=godRays.shader.getUniformID("exposure");
	godRays.glslDecay=godRays.shader.getUniformID("decay");
	godRays.glslDensity=godRays.shader.getUniformID("density");
	godRays.glslWeight=godRays.shader.getUniformID("weight");
	godRays.glslLightPositionOnScreen=godRays.shader.getUniformID("lightPositionOnScreen");
	godRays.glslScreenTexture=godRays.shader.getUniformID("screenTexture");
	///////////////////////////////////////////////////////////////////////
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
	addSun(sun.getString("image"),
		   sun.getVector3D("scale"),
		   sun.getFloat("period"));
	//get light info	
	Vec4 lightAmbient(1.0,1.0,1.0,1.0);
	Vec4 lightDiffuse(1.0,1.0,1.0,1.0);
	Vec4 lightSpecular(1.0,1.0,1.0,1.0);
	if(sun.existsAsType("light",Table::TABLE)){
		const Table& light=sun.getConstTable("light");
		lightAmbient=light.getVector4D("ambien",lightAmbient);
		lightDiffuse=light.getVector4D("diffuse",lightDiffuse);
		lightSpecular=light.getVector4D("specular",lightSpecular);
	}
	this->sun->setMaterial(lightAmbient,lightDiffuse,lightSpecular,Vec4::ZERO,1.0);
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
		if(planet.existsAsType("atmosphere",Table::TABLE)){
			const Table& atmosphere=planet.getConstTable("atmosphere");			
			DEBUG_ASSERT_MSG(atmosphere.size()==3,"PlanetsManager error : in "<< (itTable.first.isString() ? 
																						itTable.first.string() : 
																						String::toString(itTable.first.integer()))
																			  <<", atmosphere table must to be size 3");
			DEBUG_ASSERT_MSG(atmosphere.existsAsType(0,Table::STRING)&&
							 atmosphere.existsAsType(1,Table::STRING)&&
							 atmosphere.existsAsType(2,Table::STRING)
							,"PlanetsManager error : in "<< (itTable.first.isString() ? 
															 itTable.first.string() : 
															 String::toString(itTable.first.integer()))
														  <<", atmosphere table must contain 3 strings")																			
			ptr->setAtmosphereTexture(atmosphere.getString(0),
									  atmosphere.getString(1),
									  atmosphere.getString(2));
		}
		
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
		blackMesh.shader.bind();
			drawPlanetssCores();
		blackMesh.shader.unbind();
		//
	blackTexture.disableRender();
	////////////////////////////////////////////////////////////////////
	//reset viewport
	glViewport(oldViewport.x,oldViewport.y,oldViewport.z,oldViewport.w);
	
	//draw word
	drawSun();
	
	//////////////////////////////////////////////////////////////////
	//enable light
	render->enableLight();
	//load view matrix
	glLoadMatrixf(camera->getGlobalMatrix());
	//set lights
	render->setLight(sun->getPosition()+Vec3(0,000,0), sun->getAmbient(), sun->getDiffuse(), sun->getSpecular());
	//////////////////////////////////////////////////////////////////
	//draw planets
	sunLight.shader.bind();
	sunLight.uniforming();	
		drawPlanetssCores();	
	sunLight.shader.unbind();	
	//draw clouds
	sunLightCloud.shader.bind();
	sunLightCloud.uniforming();	
		drawPlanetssClouds();
	sunLightCloud.shader.unbind();
	//////////////////////////////////////////////////////////////////
	render->disableLight();
	//////////////////////////////////////////////////////////////////
	//draw atmosphere
	sunLightAtmosphere.shader.bind();
	sunLightAtmosphere.uniforming();	
		drawPlanetssAtmosphere();
	sunLightAtmosphere.shader.unbind();
	//////////////////////////////////////////////////////////////////
	//draw vbo
	godRays.shader.bind();
	godRays.uniforming();
	godRays.shader.uniformVector2D(godRays.glslLightPositionOnScreen,
								  camera->getPointIn3DSpace(sun->getPosition()));
	//additive blend
	glEnable( GL_BLEND );   
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	//
	blackTexture.draw();
	godRays.shader.unbind(); 
	//reset old blend state   
	render->setBlendState(blendState);
}
void PlanetsManager::drawPlanetssClouds(){
	for(auto planet:planets)
		planet->drawCloud(*camera);
}
void PlanetsManager::drawPlanetssAtmosphere(){
	for(auto planet:planets)
		planet->drawAtmosphere(*camera);
}
void PlanetsManager::drawPlanetssCores(){
	for(auto planet:planets)
		planet->drawPlanet(*camera);
}
void PlanetsManager::drawSun(){
	sun->draw(*camera);
}