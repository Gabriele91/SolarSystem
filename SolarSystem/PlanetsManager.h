#ifndef PLANETSMANAGER_H
#define PLANETSMANAGER_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>
#include <Planet.h>
#include <Shader.h>
#include <Table.h>
#include <SolarRender.h>

namespace SolarSystem {

	class PlanetsManager {

		Planet* sun;
		std::vector<Planet*> planets;
		Camera *camera;
		SolarRender *render;
		float scaleEllipses;
		float scaleSun;
		float scalePlanets;		
		Shader blackMesh;
		RenderTexture blackTexture;
		Table configfile; 
		///////////////////////////////
		//shader id
		struct Godrays{
			uint glslExposure,
				 glslDecay,
				 glslDensity,
				 glslWeight,
				 glslLightPositionOnScreen,
				 glslScreenTexture;
			//shader values
			float uniformExposure,
				  uniformDecay,
				  uniformDensity,
				  uniformWeight;
			//uniforming:
			void uniforming(){
				shader.uniformFloat(glslExposure,uniformExposure);
				shader.uniformFloat(glslDecay,uniformDecay);
				shader.uniformFloat(glslDensity,uniformDensity);
				shader.uniformFloat(glslWeight,uniformWeight);
				shader.uniformInt(glslScreenTexture,0);	
			}
			///////////////
			Shader shader;
			///////////////
		}godRays;
		///////////////////////////////
		struct SunLight{

			uint glPlanetTexture,
				 glPlanetNightTexture;

			void uniforming(){
				shader.uniformInt(glPlanetTexture,0);	
				shader.uniformInt(glPlanetNightTexture,1);	
			}
			///////////////
			Shader shader;
			///////////////			
		}sunLight;
		///////////////////////////////


	public:
		
		PlanetsManager(const Utility::Path& path,
					   Camera *camera,
					   SolarRender *render);
		virtual ~PlanetsManager();

		DFORCEINLINE void setScaleEllipses(float scale){
			scaleEllipses=1.0f/scale;
		}
		DFORCEINLINE void setScaleSun(float scale){
			scaleSun=(1.0f/scale)*0.5f;
		}
		DFORCEINLINE void setScalePlanets(float scale){
			scalePlanets=(1.0f/scale)*0.5f;
		}
		
		void addSun(const Utility::Path &path,
			        const Vec3& scale,
					float rotationPeriod);
		Planet* addPlanet( //texture
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
						   float shininess);
		void setData(float day);
		void draw();
		void drawPlanets();
		void drawPlanetssClouds();
		void drawPlanetssCores();
		void drawSun();
	};

};

#endif