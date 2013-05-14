#ifndef SOLARRINGS_H
#define SOLARRINGS_H

#include <Config.h>
#include <Utility.h>
#include <Object.h>
#include <Texture.h>
#include <Camera.h>
#include <SolarRender.h>

namespace SolarSystem {

	class SolarRings : public Object {
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
		void buildMesh(float nr,float fr,int sectors);
		void bindMesh();
		//
		Texture1D diffuseTexture;
		//ptr
		SolarRender *render;
		//values
		float frSize,nrSize;

	public:

		SolarRings(SolarRender *render,
				   const Utility::Path& texture,
				   float nr=0.5,
				   float fr=1.0,
				   int sectors=64);
		void drawRing();
		void drawBase(Camera& camera);
		void draw(Camera& camera);

	};

};

#endif