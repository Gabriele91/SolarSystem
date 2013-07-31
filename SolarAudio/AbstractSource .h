#ifndef ABSTRACT_SOURCE_H
#define ABSTRACT_SOURCE_H
#include <Config.h>
#include <EString.h>
#include <Utility.h>
#include <string>

namespace SolarSystem{

	class AbstractSource{
			
	protected:
		//id emissione suono openAL
        ALuint source;
		String name;
		void _SetNameFromPathName(const Utility::Path & pathfilename);

	public:

		virtual ~AbstractSource(){};
		virtual const String& getName();
		//disable/enable loop
		virtual void disableLoop()=0;
		virtual void enableLoop()=0;
		//suona
        virtual void play()=0;
        //suona in "loop"
        virtual void loop()=0;
        //ferma
        virtual void stop()=0;
        //pausa
        virtual void pause()=0;
        //imposta volume (0...1)
        virtual void volume(float volume)=0;
		//tempo di esecuzione:
		virtual float playBackTime()=0;
		virtual float duration()=0;
        //Get States
		virtual bool isPause()=0; 
        virtual bool isPlay()=0;
        virtual bool isStop()=0; 
        virtual bool isLoop()=0;
		
	};	
}

#endif