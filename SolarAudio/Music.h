#ifndef MUSIC_H
#define MUSIC_H


#include "APIAudio.h"
#include "AbstractSource .h"
#include "StreamBuffer.h"

namespace Turtles{
	namespace Audio{
        /***********************
        * Gestione audio
        ************************/
		class Music : public AbstractSource{

            //buffer file
            StreamBuffer *buffer;

            public:

            //costruttore
			Music();
            Music(const std::string & filename);
            //distruttore
            virtual ~Music();
            //carica suono
            void LoadMusic(const std::string & filename);
			//disable/enable loop
			virtual void DisableLoop();
			virtual void EnableLoop();
            //suona
            virtual void Play();
            //suona in "loop"
            virtual void Loop();
            //ferma
            virtual void Stop();
            //pausa
            virtual void Pause();
			//tempo di esecuzione:
			virtual float PlayBackTime();
			virtual float Duration();
            //Get States
            virtual bool IsPause();
            virtual bool IsPlay();
            virtual bool IsStop();
            virtual bool IsLoop();
            //imposta volume (0...1)
            virtual void Volume(float volume);


        };
    };
};

#endif