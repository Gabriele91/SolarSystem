#include <stdafx.h>
#include <Application.h>
#include <Sound.h>
//////////////////////////////////
using namespace SolarSystem;
//////////////////////////////////

//costruttore
Sound::Sound(){
	format=buffer=source=AL_NONE;
	lenSound=0;
};
Sound::Sound(const String & sound){
	format=buffer=source=AL_NONE;
	lenSound=0;
	loadSound(sound);
};
//distruttore
Sound::~Sound(){
	 alDeleteBuffers(1,&buffer);
	 alDeleteSources(1,&source);
};

extern ALuint OpenALReadWav(const Utility::Path& path,int & outformat,float & leng);
//carica suono
void Sound::loadSound(const String & sound){
//se non è vuoto, cancella
if(buffer!=AL_NONE)
	this->Sound::~Sound();
 //carica il file
 buffer = OpenALReadWav(sound,format,lenSound);

 _SetNameFromPathName(sound);
 //genera la sorgente o vi associa il buffer
 alGenSources (1, &source);
 alSourcei (source, AL_BUFFER, buffer);
}
// 
void Sound::disableLoop(){
	alSourcei(source, AL_LOOPING,  AL_FALSE );
}
void Sound::enableLoop(){	
	alSourcei(source, AL_LOOPING,  AL_TRUE );
}
//suona
void Sound::play(){
 alSourceRewind(source);
 alSourcePlay (source);
}
//suona loop
void Sound::loop(){
    if(!isPlay()) play();
    if(!isLoop()) alSourcei(source, AL_LOOPING,  AL_TRUE  );
}
//metti in pausa
void Sound::pause(){
    if(isPlay())
        alSourcePause(source);
}
//ferma ripsoduzione
void Sound::stop(){
    if(isPlay()){
        alSourcei(source, AL_LOOPING,  AL_FALSE );
        alSourceStop(source);
    }
}
//imposta volume (0...1)
void Sound::volume(float volume){
	alSourcef(source, AL_GAIN, volume);
}
//tempo di esecuzione:
float Sound::playBackTime(){
	float result = 0.0;
	alGetSourcef(source, AL_SEC_OFFSET , &result);
	return result;
}
float Sound::duration(){
	return this->lenSound;
}
//Get State
bool Sound::isPause(){
	ALint status;
	alGetSourcei (source, AL_SOURCE_STATE, &status);
	return status == AL_PAUSED;
}
bool Sound::isPlay(){
	ALint status;
	alGetSourcei (source, AL_SOURCE_STATE, &status);
	return status == AL_PLAYING;
}
bool Sound::isStop(){
	ALint status;
	alGetSourcei (source, AL_SOURCE_STATE, &status);
	return ( status != AL_PLAYING && status != AL_PAUSED );
}
bool Sound::isLoop(){	
	ALint looping;
	alGetSourcei (source, AL_LOOPING, &looping);
	return looping != 0 ;
}