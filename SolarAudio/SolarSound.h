#ifndef SOLARSOUND_H
#define SOLARSOUND_H
#include <APIAudio.h>
#include <Table.h>
#include <Sound.h>
#include "../src/Thread/Thread.h"
#include <Timer.h>

namespace SolarSystem{

	class SolarSound : protected  Thread{

	public:

		SolarSound(const Table& config)
		:Thread(TERMINATE_DELETE){
			//all null
			soundMenu=NULL;
			soundSystem=NULL;
			timeChange=config.getFloat("soundChange",1000.0f);
			//load sound			
			DEBUG_ASSERT_MSG(config.existsAsType("soundMenu",Table::STRING),"SolarSound: parameter soundMenu must to be setted");
			soundMenu=new Sound(config.getString("soundMenu"));
			DEBUG_ASSERT_MSG(config.existsAsType("soundSystem",Table::STRING),"SolarSound: parameter soundSystem must to be setted");
			soundSystem=new Sound(config.getString("soundSystem"));
		};	
		virtual ~SolarSound(){	
			if(soundSystem) delete soundSystem;
			if(soundMenu) delete soundMenu;
		}
	
		void loopMenu(){
			soundMenu->loop();
		}
		void loopSystem(){
			soundSystem->loop();
			start();
		}
		virtual int run(){
			Timer timer;
			timer.start();
			while(timer.getGetCounter()<timeChange){
				double v=timer.getGetCounter()/timeChange;
				soundSystem->volume(v);
				soundMenu->volume(1.0-v);
			}
			return 0;
		}

	private:
		Sound *soundMenu;
		Sound *soundSystem;
		double timeChange;

	};

};
#endif
