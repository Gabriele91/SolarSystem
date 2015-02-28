#ifndef SOLARSYSTEMMENU_H
#define SOLARSYSTEMMENU_H

#include <Config.h>
#include <Input.h>
#include <Application.h>
#include <SolarMenu.h>
#include <Camera.h>
#include <PlanetsManager.h>
#include <SolarRender.h>
#include <SolarFly.h>
#include <Font.h>

namespace SolarSystem {

	class SolarSystemMenu : public Input::KeyboardHandler{

		SolarMenu         menu;
		SolarFly	      cameraCtrl;
        Camera           *camera;
        PlanetsManager   *planets;
        Font             *font;
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
            Vec3    campos;
            float startAngle;
            float cameraHigth;
            String text;
            Vec2   textPos;
            Color  textColor;
            float  textTime;
        };


        float timeCounting;
        float timeRotation;
        float timeMove;

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
		virtual ~SolarSystemMenu();
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
            cameraCtrl.lock();

            if(isunlock)
            {
	            Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)&cameraCtrl);
	            Application::instance()->getInput()->addHandler((Input::MouseHandler*)&cameraCtrl);
                Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
            }
            isunlock=false;
        }
        void unlock(){
            //unlock menu
            menu.unlock();
            if(state.state==FREE_SYSTEM) cameraCtrl.unlock();

            if(!isunlock)
            {
	            Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)&cameraCtrl);
	            Application::instance()->getInput()->addHandler((Input::MouseHandler*)&cameraCtrl);
                Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
            }
            isunlock=true;
        }


	};

};

#endif
