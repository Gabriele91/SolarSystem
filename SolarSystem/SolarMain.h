#ifndef SOLARMAIN_H
#define SOLARMAIN_H
#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <Math2D.h>
#include <Camera.h>
#include <Texture.h>
#include <Shader.h>
#include <SolarRender.h>
#include <Planet.h>
#include <PlanetsManager.h>
#include <SolarFly.h>
#include <SolarMenu.h>
#include <ApplicationState.h>
#define NOT_INCLUDE_INL
#include "../src/Image/Image.h"

namespace SolarSystem {

	class SolarMain: public ApplicationState,
							Input::KeyboardHandler{
		SolarFly	   cameraCtrl;
		SolarRender    render;
		SolarMenu      menu;
		PlanetsManager system;
		Camera camera;
		Object obj;
		float days;
		float incDaysDt;
        bool  returnToMenu;

	public:
		SolarMain(Table& config)
            :menu(config.getTable("menu"))
            ,system(&camera,&render,config.getTable("solarSystem"))
            ,cameraCtrl(&camera)
            ,returnToMenu(false)
		{}
		virtual void start(){
		//input
		enableEvents();
		//init render
		render.init();
		//setup camera
		//wfactor
		float wfactor=(float)Application::instance()->getScreen()->getHeight()/Application::instance()->getScreen()->getWidth();
		camera.setPerspective(-0.5, 0.5,-0.5*wfactor,0.5*wfactor, 1.0f,100000.0f);
		Quaternion quad;
		//quad.setFromEulero(Math::torad(-90),0,0);
		//camera.setPosition(Vec3(0,-10000,0));
		quad.setFromEulero(Math::torad(0),0,0);
		camera.setPosition(Vec3(0,0,-15000));
		camera.setRotation(quad);
		//start day
		days=900;
		incDaysDt=0.2;
		/////////////////////////////////////////////
		menu.addOnClick("earth",[this](){
			this->camera.setTranslation(Vec3(0,0,300));
		});
		menu.addOnClick("sun",[this](){
			this->camera.setTranslation(Vec3(0,0,1300));
		});
		menu.addOnClick("moon",[this](){
			this->camera.setTranslation(Vec3(0,0,-300));
		});

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
			menu.update(dt);
			//draw solar
			system.draw();
			//draw gui
			menu.draw(&render);
			//font buffer
			//get errors...
			CHECK_GPU_ERRORS();
		}
		void enableEvents(){
			//input
			getInput()->addHandler((Input::KeyboardHandler*)this);
			getInput()->addHandler((Input::KeyboardHandler*)&cameraCtrl);
			getInput()->addHandler((Input::MouseHandler*)&cameraCtrl);			
		}
		void disableEvents(){
            //input
            getInput()->removeHandler((Input::KeyboardHandler*)this);
            getInput()->removeHandler((Input::KeyboardHandler*)&cameraCtrl);
            getInput()->removeHandler((Input::MouseHandler*)&cameraCtrl);		
		}
		virtual void end(){
			disableEvents();
        }
		
		virtual void onKeyPress(Key::Keyboard key) {
			if(key==Key::N1)
				cameraCtrl.setMoveVelocity(Vec3(1,1,1));
			else if(key==Key::N2)
				cameraCtrl.setMoveVelocity(Vec3(10.0,10.0,10.0));
			else if(key==Key::N3)
				cameraCtrl.setMoveVelocity(Vec3(100.0,100.0,100.0));
			else if(key==Key::N4)
				cameraCtrl.setMoveVelocity(Vec3(1000.0,1000.0,1000.0));
			else if(key==Key::P){
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