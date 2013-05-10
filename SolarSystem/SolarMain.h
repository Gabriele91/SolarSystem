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
		SolarMain(const Utility::Path& config):
			MainInstance("Solar System",1280,768,32,60,false)
			,system(config,&camera,&render)
			,cameraCtrl(&camera)
		{}
		virtual void start(){
		//input
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)&cameraCtrl);
		Application::instance()->getInput()->addHandler((Input::MouseHandler*)&cameraCtrl);
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
			incDaysDt+=(Application::instance()->getInput()->getKeyDown(Key::R)?0.2:0.0);
			incDaysDt-=(Application::instance()->getInput()->getKeyDown(Key::T)?0.2:0.0);
			incDaysDt+=(Application::instance()->getInput()->getKeyHit(Key::F)?0.1:0.0);
			incDaysDt-=(Application::instance()->getInput()->getKeyHit(Key::G)?0.1:0.0);
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
