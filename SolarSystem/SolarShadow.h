#ifndef SOLARSHADOW_H
#define SOLARSHADOW_H

#include <SolarRender.h>
#include <Config.h>
#include <Texture.h>
#include <Camera.h>
//#define SHADOW_SHOW_CAMERA

namespace SolarSystem {

	class SolarShadow {

#ifdef SHADOW_SHOW_CAMERA
		RenderTexture texture;
#else
		ShadowTexture texture;
#endif
		
		Vec4 globalViewport;
		Camera shadowLight;
		SolarRender *render;
		SolarRender::MatrixsState globalMState;

	public:
		
		SolarShadow(SolarRender* render,
					const Vec3& point=Vec3::ZERO);
		Camera& getLightCamera(){ 
			return shadowLight;
		}
		void changeDir(const Vec3& point);
		void enableRender();
		void disableRender();
		void draw();

	};

};

#endif