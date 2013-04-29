#ifndef PLANET_H
#define PLANET_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>
#include <SolarRender.h>

namespace SolarSystem {

	class Planet : public Object {
		
	
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
		//elliptical path
		Vec2 radius;
		float daysOfYear;
		float day;
		//planet rotation
		float rotationPeriod;
		//pointer to render
		SolarRender *render;
		//material
		Vec4 ambient;
		Vec4 diffuse;
		Vec4 specular;
		Vec4 emission;
		float shininess;

	public:
		
		Planet(SolarRender *render,
		       const Utility::Path& texture,
			   int rings=64,
			   int sectors=64);

		virtual ~Planet();
		//draw
		void draw(Camera& camera);
		void drawPlanet(Camera& camera);
		void drawCloud(Camera& camera);
		//set data
		void setData(float day);
		//set planet info
		void setPlanetInfo(const Vec2& radius,
						   float daysOfYear,
						   float rotationPeriod);
		//set material		
		void setMaterial(const Vec4& ambient,
						 const Vec4& diffuse,
						 const Vec4& specular,
						 const Vec4& emission,
						 float shininess);
		//set extra texture
		void setCloudTexture(const Utility::Path& texture);
		void setBlackTexture(const Utility::Path& texture);
		/*
		todo add atmosfere like night and day texture
		*/
	};
};

#endif