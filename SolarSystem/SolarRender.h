#ifndef SOLARRENDER_H
#define SOLARRENDER_H

#include <Config.h>
#include <Math3D.h>
#include <Texture.h>

namespace SolarSystem {

	class SolarRender {
		
		Texture black;
		Texture blackAlpha0;
		Texture white;
		Texture whiteAlpha0;

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
		struct MatrixsState{
			Mat4 projection,modelview;
			bool operator==(const MatrixsState& bs){
				return modelview==bs.modelview && projection==bs.projection;
			}
			bool operator!=(const MatrixsState& bs){
				return modelview!=bs.modelview || projection!=bs.projection;
			}
		};

		enum CULLFACE{ FRONT=GL_FRONT, BACK=GL_BACK };
		SolarRender();

		void init();
		void disableCullFace();
		void enableCullFace();
		bool cullFaceIsEnable();
		void setCullFace(CULLFACE type);
		void setClearColor(const Vec4& color);

		BlendState getBlendState() const;
		void setBlendState(const BlendState& bs);
		
		MatrixsState getMatrixsState() const;
		void getMatrixsState(MatrixsState& ms) const;
		void setMatrixsState(const MatrixsState& ms);
		
		void enableLight();
		void disableLight();
		bool lightIsEnable() const;

		void enableZBuffer();
		void disableZBuffer();
		bool zBufferIsEnable() const;
		
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
		Texture& getTextureBlackAlpha0(){ return blackAlpha0; }
		Texture& getTextureWhiteAlpha0(){ return whiteAlpha0; }
		const Texture& getTextureBlack()const{ return black; }
		const Texture& getTextureWhite()const{ return white; }
		const Texture& getTextureBlackAlpha0()const{ return blackAlpha0; }
		const Texture& getTextureWhiteAlpha0()const{ return whiteAlpha0; }
	};

};

#endif