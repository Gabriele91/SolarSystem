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
		//texture
		Texture texture;		 
		//elliptical path
		Vec2 radius;
		float daysOfYear;
		float day;

	public:

		Planet(const Utility::Path& texture,
			   int rings=50,
			   int sectors=50);
		//draw
		void draw(Camera& camera);
		//set data
		void setData(float day);
		//set planet info
		void setPlanetInfo(Vec2& radius,
						   float daysOfYear);
	};

};

#endif