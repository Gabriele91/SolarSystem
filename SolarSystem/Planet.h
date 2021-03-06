#ifndef PLANET_H
#define PLANET_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>
#include <SolarRender.h>
#include <SolarRings.h>
#include <Shader.h>

namespace SolarSystem {

	class Planet : public Object {
    public:

        
		struct SunLightALL{

			uint glPlanetTexture,
				 glPlanetNightTexture,
				 glPlanetSpecularTexture,
                 glCloudTexture,
                 glAtmGrad1,
				 glAtmGrad2,
				 atmRim,
                 textureMat4;

			void uniforming(){
				shader.uniformInt(glPlanetTexture,0);	
				shader.uniformInt(glPlanetNightTexture,1);	
				shader.uniformInt(glPlanetSpecularTexture,2);
				shader.uniformInt(glCloudTexture,3);	
				shader.uniformInt(glAtmGrad1,4);
				shader.uniformInt(glAtmGrad2,5);
				shader.uniformInt(atmRim,6);	
			 }
			///////////////
			Shader shader;
			///////////////			
		};
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
		};
		struct SunLightCloud{

			uint glCloudTexture,
                 textureMat4;

			void uniforming(){
				shader.uniformInt(glCloudTexture,0);	
			}
			///////////////
			Shader shader;
			///////////////			
		};
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
		};
		struct SunLightRings{

			uint glRingsTexture;

			void uniforming(){
				shader.uniformInt(glRingsTexture,0);
			}

			Shader shader;
		};

        static void initShader();

        static SunLightALL        sunLightALL;
        static SunLight           sunLight;
        static SunLightCloud      sunLightCloud;
        static SunLightAtmosphere sunLightAtmosphere;
        static SunLightRings      sunLightRings;

	private:
		//gpu buffers
		struct GLVertex{
			Vec3 vertices;
			Vec3 normals;
			Vec2 texcoords;
		};
		//
		uint verticesSize;
		uint verticesBuffer;
		//
		uint indicesSize;
		uint indicesBuffer;
		//
		void buildMesh(int rings,int sectors);
		void bindMesh();
		//texture
		Texture texture;
		Texture *cloudTexture;
		Texture *blackTexture;
		Texture *specularTexture;
		Texture1D *atmGrad1;
		Texture1D *atmGrad2;
		Texture1D *atmRim;
		//elliptical path
		Vec2 radius;
		float daysOfYear;
		float day;
		//planet rotation
		float rotationPeriod;
		Vec3 rotationNormal;
		//cloud offset;
		Vec3 cloudDayOffset;
		Vec3 cloudOffset;
        void uniformCloudOffset(Shader& shader,uint id);
		//pointer to render
		SolarRender *render;
		//material
		Vec4 ambient;
		Vec4 diffuse;
		Vec4 specular;
		Vec4 emission;
		float shininess;
		//rings
		SolarRings *rings;

	public:

		Planet(SolarRender *render,
		       const Utility::Path& texture,
			   int rings=64,
			   int sectors=64);

		virtual ~Planet();
		//draw
		void draw(Camera& camera);
		void drawSphere();
		void drawCircle();
		void drawBase(Camera& camera);
		void drawBaseRings(Camera& camera);

		void drawPlanet(Camera& camera);
		void drawCloud(Camera& camera);
		void drawAtmosphere(Camera& camera);
		void drawPlanetALL(Camera& camera);

		void drawPlanetAuto(Camera& camera);

		void drawRings(Camera& camera);
		//info planet
		bool inCamera(Camera& camera);
		//set data
		void setData(float day);
		//set planet info
		void setPlanetInfo(const Vec2& radius,
						   float daysOfYear,
						   float rotationPeriod);
		void setRotationNormal(const Vec3& rotNormal){
			rotationNormal=rotNormal;
		}
		//set cloud info
		void setCloudOffset(const Vec3& cloudOffset);
		//set material
		void setMaterial(const Vec4& ambient,
						 const Vec4& diffuse,
						 const Vec4& specular,
						 const Vec4& emission,
						 float shininess);
		//getter
		DFORCEINLINE const Vec4& getAmbient() const { return ambient; }
		DFORCEINLINE const Vec4& getDiffuse() const { return diffuse; }
		DFORCEINLINE const Vec4& getSpecular() const { return specular; }
		DFORCEINLINE const Vec4& getEmission() const { return emission; }
		DFORCEINLINE float getShininess() const { return shininess; }
		//set extra texture
		void setCloudTexture(const Utility::Path& texture);
		void setBlackTexture(const Utility::Path& texture);
		void setSpecularTexture(const Utility::Path& texture);
		void setAtmosphereTexture(const Utility::Path& grad1,
								  const Utility::Path& grad2,
								  const Utility::Path& rim);
		void setRings(const Utility::Path& texture,
			          float nr=0.5,
					  float fr=1.0,
					  const Vec3& rotationOffSet=Vec3::ZERO);
		/*
		todo add atmosfere like night and day texture
		*/
	};
};

#endif
