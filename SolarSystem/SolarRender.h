#ifndef SOLARRENDER_H
#define SOLARRENDER_H

#include <Config.h>
#include <Math2D.h>
#include <Texture.h>

namespace SolarSystem {

	class SolarRender {
		
		Texture black;
		Texture white;

	public:

		struct BlendState{
			int src,dst,enable;
			bool operator==(const BlendState& bs){
				return src==bs.src && dst==bs.dst && enable == bs.enable;
			}
			bool operator!=(const BlendState& bs){
				return src!=bs.src || dst!=bs.dst || enable != bs.enable;
			}
		};

		enum CULLFACE{ FRONT=GL_FRONT, BACK=GL_BACK };
		SolarRender();

		void init();
		void setCullFace(CULLFACE type);
		void setClearColor(const Vec4& color);

		BlendState getBlendState();
		void setBlendState(const BlendState& bs);
		
		void enableLight();
		void disableLight();
		bool lightIsEnable();

		void enableZBuffer();
		void disableZBuffer();
		bool zBufferIsEnable();
		
		void setLight(const Vec3& posiction,
					  const Vec4& ambient,
					  const Vec4& diffuse,
					  const Vec4& specular);
		void setLightAttenuation(float constant,
								 float linear,
								 float quantistic);
		void setMaterial(const Vec4& ambient,
						 const Vec4& diffuse,
						 const Vec4& specular,
						 const Vec4& emission,
						 float shininess);
		
		Texture& getTextureBlack(){ return black; }
		Texture& getTextureWhite(){ return white; }
	};

};

#endif