#ifndef PLANET_H
#define PLANET_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>

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
		//elliptical path
		Vec2 radius;
		float daysOfYear;
		float day;
		//planet rotation
		float rotationPeriod;

	public:
		
		Planet(const Utility::Path& texture,
			   int rings=50,
			   int sectors=50);
		Planet(const Utility::Path& texture,
			   const Utility::Path& textureCloud,
			   int rings=50,
			   int sectors=50);
		virtual ~Planet();
		//draw
		void draw(Camera& camera);
		//set data
		void setData(float day);
		//set planet info
		void setPlanetInfo(const Vec2& radius,
						   float daysOfYear,
						   float rotationPeriod);
	};

};

#endif