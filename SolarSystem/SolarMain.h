#include <Config.h>
#include <Application.h>
#include <MainInstance.h>

namespace SolarSystem {
	class SolarMain: public MainInstance{

	
	public:
		SolarMain():MainInstance("Solar System",1280,786){}
		virtual void start(){
		}
		virtual void run(float dt){
		}
		virtual void end(){
		}

	};
};
