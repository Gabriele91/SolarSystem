#ifndef SOLARSHADOW_H
#define SOLARSHADOW_H

#include <SolarRender.h>
#include <Config.h>
#include <Texture.h>
#include <Shader.h>
#include <Camera.h>
#include <Planet.h>
//#define SHADOW_SHOW_CAMERA

namespace SolarSystem {

	class SolarShadow {

		ShadowTexture texture;		
		Vec4 globalViewport;
		Camera shadowLight;
		SolarRender *render;
		SolarRender::MatrixsState globalMState;
		Shader shadowTextureShader;
		struct ShadowShader{
		
			uint glDelphMVP,
				 glShadowMap,
				 glIntesity;

			Shader shader;

		}shadowShader;

	public:
		
		SolarShadow(SolarRender* render,
					const Vec3& point=Vec3::ZERO,
					uint width=512,
					uint height=512,
					const Vec2& xyFactor=Vec2(.25,.25));
		Camera& getLightCamera(){ 
			return shadowLight;
		}
		void changeDir(const Vec3& point);
		void madeShadowMap(Planet *planet,const Vec3& addScale=Vec3::ONE);
		void drawShadow(Camera *camera,Planet *planet,float intesity=1.0);

		void draw();

	};

};

#endif