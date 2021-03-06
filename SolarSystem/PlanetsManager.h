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
#include <SolarRings.h>

namespace SolarSystem {

	class PlanetsManager {

		Planet* sun;
		////////////////////
		struct SolarShadowObjects{
			SolarShadow* shadowLight;
			Planet* source,*dest,*target;
			Vec3 sizeObjectInShadow;
			float intesity;
			void drawShadow(Camera *camera){
				if(dest->inCamera(*camera)){
					//make shadow	
					shadowLight->changeDir(target->getPosition(true));
					shadowLight->madeShadowMap(source,sizeObjectInShadow);	
					//draw shadow map
					shadowLight->drawShadow(camera,dest,intesity);	
				}
			}
		};
		std::vector<SolarShadowObjects> shadows;
		////////////////////
		DUNORDERED_MAP<String,Planet*> planets;
		Camera *camera;
		SolarRender *render;
		SolarSky *skybox;
		float scaleEllipses;
		float scaleSun;
		float scalePlanets;	
		RenderTexture blackTexture;
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
		struct Fxaa{
			uint glScreenTexture;
			uint glTexcoordOffset;

			Vec2 invScreen;

			void uniforming(){
				shader.uniformInt(glScreenTexture,0);
				shader.uniformVector2D(glTexcoordOffset,invScreen);
			}
		
			Shader shader;
		}fxaa;		
		RenderTexture worldTexture;
		bool enableFxaa;
		///////////////////////////////

	public:
		
		PlanetsManager(Camera *camera,
					   SolarRender *render,
					   const Table& configfile);
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
		DFORCEINLINE void setFxaa(bool enable){
			enableFxaa=enable;
		}
		DFORCEINLINE bool fxaaIsEnable(){
			return enableFxaa;		
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
		void drawPlanetssBases();
		void drawPlanetssCores();
		void drawPlanetssAtmosphere();
		void drawPlanetssRings();
		void drawSun();

		Planet& getPlanet(const String& name){
			return *(planets[name]);
		}
		Planet& getSun(){
			return *(sun);
		}
        
        DUNORDERED_MAP<String,Planet*>::iterator begin(){
          return  planets.begin();
        }
        DUNORDERED_MAP<String,Planet*>::iterator end(){
            return planets.end();
        }
        
	};

};

#endif