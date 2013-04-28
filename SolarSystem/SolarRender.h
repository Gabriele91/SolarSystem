#ifndef SOLARRENDER_H
#define SOLARRENDER_H

#include <Config.h>
#include <Math2D.h>

namespace SolarSystem {

	class SolarRender {

	public:
		
		enum CULLFACE{ FRONT=GL_FRONT, BACK=GL_BACK };
		SolarRender();

		void init();
		void setCullFace(CULLFACE type);
		void setClearColor(const Vec4& color);

		void enableLight();
		void disableLight();
		void setLight(const Vec3& posiction,
					  const Vec4& ambient,
					  const Vec4& diffuse,
					  const Vec4& specular);
		void setMaterial(const Vec4& ambient,
						 const Vec4& diffuse,
						 const Vec4& specular,
						 const Vec4& emission,
						 float shininess);

	};

};

#endif