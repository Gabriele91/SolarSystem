#ifndef SOLARFLY_H
#define SOLARFLY_H

#include <Config.h>
#include <Application.h>
#include <Math3D.h>
#include <Camera.h>
#include <Gamepad/SolarPad.h>

namespace SolarSystem {

	class SolarFly : Input::KeyboardHandler,
					 Input::MouseHandler,
                     GamepadManager::Gamepad
    {

		Camera *camera;
		Vec2 turnVel;
		Vec3 movVel;
        bool isLocked;
        GamepadManager pads;

	public:

		SolarFly(Camera *camera);
        ~SolarFly();
		//setter and getter
		DFORCEINLINE void setTurnVelocity(const Vec2& tv){ turnVel=tv; }
		DFORCEINLINE const Vec2& getTurnVelocity(){ return turnVel; }
		DFORCEINLINE void setMoveVelocity(const Vec3& mv){ movVel=mv; }
		DFORCEINLINE const Vec3& getMoveVelocity(){ return movVel; }
        //update
        void update();
		//hendler input
		virtual void onKeyDown(Key::Keyboard key);
		virtual void onMouseDown(Vec2 mousePosition, Key::Mouse button);
        //pad
        virtual void onButtonDown(uint button, double timestamp);
        virtual void onButtonUp(uint button,  double timestamp);
        virtual void onAxisMove(uint axisID, float value, float lastValue, double timestamp);
        //
        void lock();
        void unlock();

	};

};

#endif