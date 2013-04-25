#ifndef MAININSTANCE_H
#define MAININSTANCE_H

#include <Config.h>
#include <Types.h>
#include <Screen.h>
#include <EString.h>

namespace SolarSystem {

    /** 
    * Abstract class, represent the main application instance 
	* @class MainInstance <MainInstance.h>
    */
	class MainInstance {
	public:

		MainInstance(const String& name,
					 uint width, 
					 uint height, 
					 uint bites=32,
					 uint FreamPerSecond=60,
					 bool fullscreen=false);

		virtual ~MainInstance(){};
		virtual void start()=0;
		virtual void run(float dt)=0;
		virtual void end()=0;

	private:

	};

};

#endif