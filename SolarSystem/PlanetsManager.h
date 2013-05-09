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
#include <SolarSky.h>
#include <SolarShadow.h>

namespace SolarSystem {

	class PlanetsManager {

		Planet* sun;
		DUNORDERED_MAP<String,Planet*> planets;
		Camera *camera;
		SolarRender *render;
		SolarSky *skybox;
		SolarShadow solarShadow;
		float scaleEllipses;
		float scaleSun;
		float scalePlanets;	
		RenderTexture blackTexture;
		Table configfile; 
		bool enableBloom,enableGodRays;
		Vec3 attenuation;
		///////////////////////////////
		struct BlackMesh{
			Shader shader;
		}blackMesh;
		///////////////////////////////
		struct Bloom{	
			uint glslScreenTexture,
				 glslInvSizeScreenMulQuality;
			//shader values
			Vec2 uniformInvSizeScreenMulQuality;
			//uniforming:
			void uniforming(){
				shader.uniformVector2D(glslInvSizeScreenMulQuality,
									   uniformInvSizeScreenMulQuality);
				shader.uniformInt(glslScreenTexture,0);	
			}
			///////////////
			Shader shader;
			///////////////
		}bloom;
		///////////////////////////////
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
				 glPlanetNightTexture,
				 glPlanetSpecularTexture;

			void uniforming(){
				shader.uniformInt(glPlanetTexture,0);	
				shader.uniformInt(glPlanetNightTexture,1);	
				shader.uniformInt(glPlanetSpecularTexture,2);	
			}
			///////////////
			Shader shader;
			///////////////			
		}sunLight;
		struct SunLightCloud{

			uint glCloudTexture;

			void uniforming(){
				shader.uniformInt(glCloudTexture,0);	
			}
			///////////////
			Shader shader;
			///////////////			
		}sunLightCloud;
		struct SunLightAtmosphere{
			
			uint glAtmGrad1,
				 glAtmGrad2,
				 atmRim;

			void uniforming(){
				shader.uniformInt(glAtmGrad1,0);
				shader.uniformInt(glAtmGrad2,1);
				shader.uniformInt(atmRim,2);
			}
			///////////////
			Shader shader;
			///////////////			
		}sunLightAtmosphere;
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
		Planet* addPlanet( const String & name,
						   //texture
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
		void drawPlanetssClouds();
		void drawPlanetssCores();
		void drawPlanetssAtmosphere();
		void drawSun();
	};

};

#endif