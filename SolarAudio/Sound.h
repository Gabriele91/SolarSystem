#ifndef SOUND_H
#define SOUND_H

#include <Config.h>
#include <AbstractSource .h>

namespace SolarSystem{
    /***********************
    * Gestione audio
    ************************/
	class Sound : public AbstractSource{

        //buffer file
        ALuint buffer;
		//format buffer
		int format;
		//buffer len in second
		float lenSound;

        public:

        //costruttore
		Sound();
		Sound(const String & sound);
        //distruttore
		virtual ~Sound();
        //carica suono
        void loadSound(const String & sound);
		//disable/enable loop
		virtual void disableLoop();
		virtual void enableLoop();
        //suona
        virtual void play();
        //suona in "loop"
        virtual void loop();
        //ferma
        virtual void stop();
        //pausa
        virtual void pause();
        //imposta volume (0...1)
        virtual void volume(float volume);
		//tempo di esecuzione:
		virtual float playBackTime();
		virtual float duration();
        //Get States			
		virtual bool isPause(); 
        virtual bool isPlay();
        virtual bool isStop(); 
        virtual bool isLoop();


    };
};
#endif
