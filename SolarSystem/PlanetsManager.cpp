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
	,skybox(render,
	        "img/sky/top.png",
	        "img/sky/bottom.png",
	        "img/sky/front.png",
	        "img/sky/back.png",
	        "img/sky/left.png",
	        "img/sky/right.png")
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
	//enable bloom
	if(enableBloom=(sun.getString("enableBloom","true")=="true")){
		//BLOOM 
		//get shader info:
		String samplesBloom("NUM_SAMPLES 12");	
		Vec2 screenInv(1.0f/Application::instance()->getScreen()->getWidth(),
					   1.0f/Application::instance()->getScreen()->getHeight());
		float quality=4.0f;
		bloom.uniformInvSizeScreenMulQuality=screenInv*quality;
		if(sun.existsAsType("bloom",Table::TABLE)){
			const Table& bloomsh=sun.getConstTable("bloom");
			samplesBloom=("NUM_SAMPLES "+String::toString((int)bloomsh.getFloat("samples",12)));
			bloom.uniformInvSizeScreenMulQuality=screenInv*bloomsh.getFloat("quality",quality);
		}
		//////////////////////////////load shader//////////////////////////////
		const char* defines[]={samplesBloom,NULL};
		bloom.shader.loadShader("shader/bloom.vs","shader/bloom.ps",defines);
		bloom.glslScreenTexture=bloom.shader.getUniformID("screenTexture");
		bloom.glslInvSizeScreenMulQuality=bloom.shader.getUniformID("invSizeScreenMulQuality");
	}
	//enable god rays
	if(enableGodRays=(sun.getString("enableGodRays","true")=="true")){
		//GOD RAYS
		//get shader info:
		String samplesGodRays("NUM_SAMPLES 100");	
		godRays.uniformExposure = 0.0044f;
		godRays.uniformDecay = 1.0f;
		godRays.uniformDensity = 1.f;
		godRays.uniformWeight = 3.65f;
		if(sun.existsAsType("godRays",Table::TABLE)){
			const Table& godRaysSh=sun.getConstTable("godRays");
			samplesGodRays=("NUM_SAMPLES "+String::toString((int)godRaysSh.getFloat("samples",100)));		
			godRays.uniformExposure =godRaysSh.getFloat("exposure",godRays.uniformExposure);
			godRays.uniformDecay =godRaysSh.getFloat("decady",godRays.uniformDecay);
			godRays.uniformDensity =godRaysSh.getFloat("density",godRays.uniformDensity);
			godRays.uniformWeight = godRaysSh.getFloat("weight",godRays.uniformWeight);
		}
		//////////////////////////////load shader//////////////////////////////
		const char* defines[]={samplesGodRays,NULL};
		godRays.shader.loadShader("shader/godRays.vs","shader/godRays.ps",defines);
		//get uniform
		godRays.glslExposure=godRays.shader.getUniformID("exposure");
		godRays.glslDecay=godRays.shader.getUniformID("decay");
		godRays.glslDensity=godRays.shader.getUniformID("density");
		godRays.glslWeight=godRays.shader.getUniformID("weight");
		godRays.glslLightPositionOnScreen=godRays.shader.getUniformID("lightPositionOnScreen");
		godRays.glslScreenTexture=godRays.shader.getUniformID("screenTexture");
	}
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
		
		if(planet.existsAsType("cloudOffset",Table::VECTOR3D))
			ptr->setCloudOffset(planet.getVector3D("cloudOffset"));
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
	//draw skybox
	render->setClearColor(Vec4(Vec3::ZERO,1.0f));
	skybox.draw(*camera);
	//clear screen
	//////////////////////////////////////////////////////////////////
	//draw word
	drawSun();
	//////////////////////////////////////////////////////////////////
	//enable light
	render->enableLight();
	//load view matrix
	glLoadMatrixf(camera->getGlobalMatrix());
	//set lights
	render->setLight(sun->getPosition(), sun->getAmbient(), sun->getDiffuse(), sun->getSpecular());
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
	if(enableGodRays||enableBloom){

		////////////////////////////////////////////////////////////////////
		//save viewport
		Vec4 oldViewport;
		glGetFloatv(GL_VIEWPORT,&oldViewport.x);
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
		//reset viewport
		glViewport(oldViewport.x,oldViewport.y,oldViewport.z,oldViewport.w);
		////////////////////////////////////////////////////////////////////

		//save blend
		auto blendState=render->getBlendState();
		//additive blend
		glEnable( GL_BLEND );   
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		//disable zbuffer
		render->disableZBuffer();
		////////////////////////////////////////////////////////////////////
		if(enableBloom){
			//BLOOM	
			bloom.shader.bind();
			bloom.uniforming();
			blackTexture.draw();
			bloom.shader.unbind();
		}
		if(enableGodRays){
			//GOD RAYS	
			godRays.shader.bind();
			godRays.uniforming();
			godRays.shader.uniformVector2D(godRays.glslLightPositionOnScreen,
										  camera->getPointIn3DSpace(sun->getPosition()));
			blackTexture.draw();
			godRays.shader.unbind();
		}
		////////////////////////////////////////////////////////////////////
		//reset old state   
		render->enableZBuffer();
		render->setBlendState(blendState);
	
	}
	
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