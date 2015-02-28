#ifndef SOLARMAIN_H
#define SOLARMAIN_H
#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <Math3D.h>
#include <Camera.h>
#include <Texture.h>
#include <Shader.h>
#include <SolarRender.h>
#include <Planet.h>
#include <PlanetsManager.h>
#include <SolarFly.h>
#include <SolarMenu.h>
#include <ApplicationState.h>
#include <SolarSystemMenu.h>
#include <Gamepad/SolarPad.h>
#define NOT_INCLUDE_INL
#include "../src/Image/Image.h"

namespace SolarSystem {

	class SolarMain: public ApplicationState,
							Input::KeyboardHandler {

		SolarRender    render;
		PlanetsManager system;
        Camera camera;
		Object obj;
		float days;
		float incDaysDt;
        bool  returnToMenu;
        SolarSystemMenu menu;
   
	public:
		SolarMain(Table& config)
            :system(&camera,&render,config.getTable("solarSystem"))
            ,returnToMenu(false)
            ,menu(&camera,&system,config.getTable("menu"))
		{}
		virtual void start(){
		//input
        unlock();
		//init render
		render.init();
        //start day
        days=900;
        incDaysDt=0.1;
        system.setData(days);
		//setup camera
		//wfactor
		float wfactor=(float)Application::instance()->getScreen()->getHeight()/Application::instance()->getScreen()->getWidth();
		camera.setPerspective(-0.5, 0.5,-0.5*wfactor,0.5*wfactor, 1.0f,300000.0f);
		//start pos camera
        float maxleng=0.0;
        for(auto planet:system)
            maxleng=Math::max(maxleng,planet.second->getPosition(true).length());
        Quaternion quad;
		quad.setFromEulero(Math::torad(-45),0,0);
		camera.setPosition(Vec3(0,maxleng,maxleng));
        camera.setRotation(quad);
		//start day
		days=900;
		incDaysDt=0.1;
		/////////////////////////////////////////////

		}
		virtual void run(float dt){
			//debug input
			incDaysDt+=(getInput()->getKeyDown(Key::R)?0.2:0.0);
			incDaysDt-=(getInput()->getKeyDown(Key::T)?0.2:0.0);
			incDaysDt+=(getInput()->getKeyHit(Key::F)?0.1:0.0);
			incDaysDt-=(getInput()->getKeyHit(Key::G)?0.1:0.0);
			//game logic
			days+=incDaysDt*dt;
			system.setData(days);
			//game logic
			menu.update(dt);
			//draw solar
			system.draw();
			//draw gui
			menu.draw(&render);
			//font buffer
			//get errors...
			CHECK_GPU_ERRORS();
		}
		void unlock(){
			//input
			getInput()->addHandler((Input::KeyboardHandler*)this);
            //enable menu
            menu.unlock();
		}
		void lock(){
            //input
            getInput()->removeHandler((Input::KeyboardHandler*)this);
            //menu look
            menu.lock();
		}
		virtual void end(){

            //lock
			lock();
        }
		
		virtual void onKeyPress(Key::Keyboard key) {
            if(key==Key::P){
				auto img=Image::getImageFromScreen(
					getScreen()->getWidth(),
					getScreen()->getHeight()
					);					
					Utility::Path filePath("Screen/screen.tga");
					for(int i=0;filePath.existsFile();++i){
						filePath=Utility::Path("Screen/screen"+String::toString	(i)+".tga");
					}
					img->save(filePath);
				delete img;
			}			
			else if(key==Key::O){
				system.setFxaa(!system.fxaaIsEnable());
			}

		}
		virtual void onKeyDown(Key::Keyboard key) {
		
			if(key==Key::X){
				Quaternion rot;
				rot.setLookRotation(-camera.getPosition(),Vec3(0,1,0));
				camera.setRotation(rot);  
			}
			else if(key==Key::E){
				Quaternion rot;
				rot.setLookRotation(-system.getPlanet("earth").getPosition()
									-camera.getPosition(),
									Vec3(0,1,0));
				camera.setRotation(rot);  
			}
		
		}
		virtual void onKeyRelease(Key::Keyboard key) {
			//exit event
			if(key==Key::ESCAPE)
				returnToMenu=true;
		}
        bool doReturnToMenu(){
            bool returnToMenu_tmp=returnToMenu;
            returnToMenu=false;
            return returnToMenu_tmp;
        }
	
	};
};
#endif