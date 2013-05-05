#ifndef SOLARFLY_H
#define SOLARFLY_H

#include <Config.h>
#include <Application.h>
#include <Math2D.h>
#include <Camera.h>

namespace SolarSystem {

	class SolarFly : Input::KeyboardHandler,
					 Input::MouseHandler{

		Camera *camera;
		Vec2 turnVel;
		Vec3 movVel;

	public:

		SolarFly(Camera *camera)
			    :camera(camera)
				,turnVel(0.1,0.1)
				,movVel(10.0,10.0,10.0){}
		//setter and getter
		DFORCEINLINE void setTurnVelocity(const Vec2& tv){ turnVel=tv; }
		DFORCEINLINE const Vec2& getTurnVelocity(){ return turnVel; }
		DFORCEINLINE void setMoveVelocity(const Vec3& mv){ movVel=mv; }
		DFORCEINLINE const Vec3& getMoveVelocity(){ return movVel; }
		//hendler input
		virtual void onKeyDown(Key::Keyboard key);
		virtual void onMouseDown(Vec2 mousePosition, Key::Mouse button);

	};

};

#endif