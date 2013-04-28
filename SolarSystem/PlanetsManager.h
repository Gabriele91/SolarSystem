#ifndef PLANETSMANAGER_H
#define PLANETSMANAGER_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>
#include <Planet.h>
#include <Shader.h>
#include <SolarRender.h>

namespace SolarSystem {

	class PlanetsManager {

		Planet* sun;
		std::vector<Planet*> planets;
		Camera *camera;
		SolarRender *render;
		float scaleElipses;
		float scaleSun;
		float scalePlanets;		
		Shader blackMesh;
		Shader godRays;
		Shader sunLight;
		RenderTexture blackTexture;
		//shader id
		uint glslExposure,
			 glslDecay,
			 glslDensity,
			 glslWeight,
			 glslLightPositionOnScreen,
			 glslScreenTexture;

	public:
		
		PlanetsManager(Camera *camera,SolarRender *render);
		virtual ~PlanetsManager();

		DFORCEINLINE void setScaleElipses(float scale){
			scaleElipses=1.0/scale;
		}
		DFORCEINLINE void setScaleSun(float scale){
			scaleSun=(1.0/scale)*0.5;
		}
		DFORCEINLINE void setScalePlanets(float scale){
			scalePlanets=(1.0/scale)*0.5;
		}
		
		void addSun(const Utility::Path &path,
			        const Vec3& scale,
					float rotationPeriod);
		void addPlanet(const Utility::Path &path,
					   const Vec2& elipse,
					   const Vec3& scale,
					   float daysInYear,
					   float rotationPeriod);
		void addPlanet(const Utility::Path &path,
					   const Utility::Path &cloudPath,
					   const Vec2& elipse,
					   const Vec3& scale,
					   float daysInYear,
					   float rotationPeriod);
		void setData(float day);
		void draw();
		void drawPlanets();
		void drawSun();
	};

};

#endif