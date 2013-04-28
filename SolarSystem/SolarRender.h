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

	};

};

#endif