#ifndef SOLARSOUND_H
#define SOLARSOUND_H

#include <APIAudio.h>
#include <Table.h>
#include <Sound.h>
#include <Timer.h>
#include "../src/Thread/Thread.h"
#include "../src/Thread/Mutex.h"

namespace SolarSystem{

    
	class SolarSound : protected  Thread{
        
        bool isrunning;
        Mutex soundmutex;

	public:

		SolarSound(const Table& config);
		virtual ~SolarSound();
	
		void loopMenu();
		void loopSystem();
		virtual int run();

	private:
		Sound *soundMenu;
		Sound *soundSystem;
		double timeChange;

	};

};
#endif
