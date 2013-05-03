#ifndef SOLARSKY_H
#define SOLARSKY_H

#include <Config.h>
#include <Texture.h>
#include <Utility.h>
#include <Camera.h>
#include <SolarRender.h>

namespace SolarSystem {

	class SolarSky {
		//
		SolarRender *render;
		//
		Texture top,
				bottom,
				front,
				back,
				left,
				right;
		//opengl info
		struct BuffersSkybox{
			uint top,
				 bottom,
				 front,
				 back,
				 left,
				 right;
		}buffer;
		//
		void buildSkyBox();
		//
	public:

		SolarSky(SolarRender *render,
				 const Utility::Path& top,
				 const Utility::Path& bottom,
				 const Utility::Path& front,
				 const Utility::Path& back,
				 const Utility::Path& left,
				 const Utility::Path& right);

		void draw(Camera& camera);
	};

};

#endif