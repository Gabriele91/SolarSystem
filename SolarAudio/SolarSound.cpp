//
//  SolarSound.cpp
//  SolarSystem
//
//  Created by Gabriele Di Bari on 04/08/13.
//
//
#include <stdafx.h>
#include "../src/Thread/Thread.h"
#include <SolarSound.h>
#include <Debug.h>
using namespace SolarSystem;

SolarSound::SolarSound(const Table& config)
                     :Thread(TERMINATE_DELETE){
    //all null
    isrunning=false;
    soundMenu=NULL;
    soundSystem=NULL;
    timeChange=config.getFloat("soundChange",1000.0f);
    //load sound
    DEBUG_ASSERT_MSG(config.existsAsType("soundMenu",Table::STRING),"SolarSound: parameter soundMenu must to be setted");
    soundMenu=new Sound(config.getString("soundMenu"));
    DEBUG_ASSERT_MSG(config.existsAsType("soundSystem",Table::STRING),"SolarSound: parameter soundSystem must to be setted");
    soundSystem=new Sound(config.getString("soundSystem"));
};
SolarSound::~SolarSound(){
    if(isrunning) {
        soundmutex.lock();
            isrunning=false;
        soundmutex.unlock();
        join();
    }
    if(soundSystem) delete soundSystem;
    if(soundMenu) delete soundMenu;
}

void SolarSound::loopMenu(){
    if(isrunning) {
        soundmutex.lock();
            isrunning=false;
        soundmutex.unlock();
        join();
    }
    soundSystem->stop();
    soundMenu->loop();
    soundMenu->volume(1.0f);
}
void SolarSound::loopSystem(){
    if(isrunning) {
        soundmutex.lock();
            isrunning=false;
        soundmutex.unlock();
        join();
    }
    soundSystem->loop();
    soundSystem->volume(0.0f);
    start();
}
int SolarSound::run(){
    //sound loop true
    soundmutex.lock();
        isrunning=true;
    soundmutex.unlock();
    //
    Timer timer;
    timer.start();
    while(timer.getGetCounter()<timeChange && isrunning){
        double v=timer.getGetCounter()/timeChange;
        soundSystem->volume(v);
        soundMenu->volume(Math::max(1.0-v,0.0));
    }
    //in the end...
    soundMenu->stop();
    soundSystem->volume(1.0f);
    //sound loop false
    soundmutex.lock();
        isrunning=false;
    soundmutex.unlock();
    //
    return 0;
}
