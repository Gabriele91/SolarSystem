#ifndef SOLARSYSTEMMENU_H
#define SOLARSYSTEMMENU_H

#include <Config.h>
#include <Input.h>
#include <Application.h>
#include <SolarMenu.h>
#include <Camera.h>
#include <PlanetsManager.h>
#include <SolarRender.h>

namespace SolarSystem {

	class SolarSystemMenu : public Input::KeyboardHandler{
        
		SolarMenu         menu;
        Camera           *camera;
        PlanetsManager   *planets;
        bool              isunlock;
        
        enum StateValues{
            POIN_TO_PLANET,
            GO_TO_PLANET, 
            ON_PLANET,
            FREE_SYSTEM,
        };
        
        struct State{
            StateValues state;
            Planet* planet;
        };
        
        
        float timeCounting;
        float timeRotation;
        float timeMove;
        
        float startAngle;
        float turnAngle;
        float keyAngle;
        
        void pointToPlanet(float dt);
        void goToPlanet(float dt);
        void onPlanet(float dt);
        
        State state;
        
	public:
		SolarSystemMenu(Camera* camera,
                        PlanetsManager* planets,
                        const Table& menuConfig);
        void update(float dt);
        void draw(SolarRender *render);
        
        virtual void onKeyDown(Key::Keyboard key){
            if(Key::LEFT==key || Key::A==key) keyAngle=-30;
            else if(Key::RIGHT==key || Key::D==key) keyAngle=30;
        }
        
        virtual void onKeyRelease(Key::Keyboard key){
            if(Key::LEFT==key || Key::A==key)  keyAngle=0;
            else if(Key::RIGHT==key || Key::D==key) keyAngle=0;
        }
        
        
        void lock(){
            //lock menu
            menu.lock();
            if(isunlock) Application::instance()->getInput()->removeHandler((Input::KeyboardHandler*)this);
            isunlock=false;
        }
        void unlock(){
            //unlock menu
            menu.unlock();
            if(!isunlock) Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
            isunlock=true;
        }
        
        
	};

};

#endif