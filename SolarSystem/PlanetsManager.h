#ifndef PLANETSMANAGER_H
#define PLANETSMANAGER_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>
#include <Planet.h>

namespace SolarSystem {

	class PlanetsManager {

		Planet* sun;
		std::vector<Planet*> planets;
		Camera *camera;
		float scaleElipses;
		float scaleSun;
		float scalePlanets;

	public:
		
		PlanetsManager(Camera *camera);
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
		
		void addSun(const Utility::Path &path,const Vec3& scale);
		void addPlanet(const Utility::Path &path,
					   const Vec2& elipse,
					   const Vec3& scale,
					   float daysInYear);
		void setData(float day);
		void draw();
		void drawPlanets();
		void drawSun();
	};

};

#endif