//
//  ApplicationState.h
//  SolarSystem
//
//  Created by Gabriele Di Bari on 04/08/13.
//
//

#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include <Config.h>
#include <Types.h>
#include <EString.h>


namespace SolarSystem {
    
    class Screen;
    class Audio;
    class Input;

    class ApplicationState{
        
    public:
        virtual ~ApplicationState(){}
        virtual void start()=0;
        virtual void run(float dt)=0;
        virtual void end()=0;
    
        Screen* getScreen();
        Input* getInput();
        Audio* getAudio();
    
    };
}

#endif
