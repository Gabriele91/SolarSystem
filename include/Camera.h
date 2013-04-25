#ifndef CAMERA_H
#define CAMERA_H

#include <Config.h>
#include <Object.h>


namespace SolarSystem {

	class Camera : public Object {

		struct{
			float dist;
			float w;
			float h;
		}plNear,
		 plFar;

	public:


		Camera():Object(){}
		//culling methods
		int sphereInFrustum(const Vec3& point, float radius);
		int pointInFrustum(Vec3 &point);
		//
	};

};

#endif