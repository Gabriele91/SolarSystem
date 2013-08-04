#ifndef MAININSTANCE_H
#define MAININSTANCE_H

#include <Config.h>
#include <Types.h>
#include <EString.h>
#include <ApplicationState.h>


namespace SolarSystem {
	
    /** 
    * Abstract class, represent the main application instance 
	* @class MainInstance <MainInstance.h>
    */
	class MainInstance : public ApplicationState {
	public:

		MainInstance(const String& name,
					 uint width, 
					 uint height, 
					 uint bites=32,
					 uint FreamPerSecond=60,
					 bool fullscreen=false,
					 int AntiAliasing=0);
		virtual ~MainInstance(){};

	};

};

#endif