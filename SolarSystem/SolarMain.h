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
#define NOT_INCLUDE_INL
#include "../src/Image/Image.h"

namespace SolarSystem {

	class SolarMain: public MainInstance,
							Input::KeyboardHandler{
		SolarFly	   cameraCtrl;
		SolarRender    render;
		PlanetsManager system;
		Camera camera;
		Object obj;
		float days;
		float incDaysDt;

	public:
		SolarMain(Table& config):
			MainInstance("Solar System",
						 (int)config.getFloat("width",640),
						 (int)config.getFloat("height",480),
						 (int)config.getFloat("bits",32),
						 (int)config.getFloat("fps",60),
						 config.getString("fullscreen","false")=="true",
						 (int)config.getFloat("MSAA",0))
			,system(&camera,&render,config.getTable("configureFile"))
			,cameraCtrl(&camera)
		{}
		virtual void start(){
		//input
		getInput()->addHandler((Input::KeyboardHandler*)this);
		getInput()->addHandler((Input::KeyboardHandler*)&cameraCtrl);
		getInput()->addHandler((Input::MouseHandler*)&cameraCtrl);
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
		}
		virtual void run(float dt){
			incDaysDt+=(getInput()->getKeyDown(Key::R)?0.2:0.0);
			incDaysDt-=(getInput()->getKeyDown(Key::T)?0.2:0.0);
			incDaysDt+=(getInput()->getKeyHit(Key::F)?0.1:0.0);
			incDaysDt-=(getInput()->getKeyHit(Key::G)?0.1:0.0);
			days+=incDaysDt*dt;
			//font buffer
			system.setData(days);
			system.draw();
			//get errors...
			CHECK_GPU_ERRORS();
		}
		virtual void end(){}
		
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
				Application::instance()->exit();
		}
	
	};
};
