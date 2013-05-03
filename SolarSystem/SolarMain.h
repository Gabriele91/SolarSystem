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

namespace SolarSystem {

	class SolarMain: public MainInstance,
							Input::KeyboardHandler,
							Input::MouseHandler{

		SolarRender    render;
		PlanetsManager system;
		Camera camera;
		Object obj;
		float days;
		float incDaysDt;

	public:
		SolarMain():
			MainInstance("Solar System",1280,720,32,60,false)
			,system("SolarSystem.conf",&camera,&render)
		{
		
		}
		virtual void start(){
		//input
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
		Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
		//init render
		render.init();
		//setup camera
		camera.setPerspective(45.0f,2.0f,1.0f,100000.0f);
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
		virtual void end(){
		}
		
		virtual void onKeyPress(Key::Keyboard key){
			float v=(Application::instance()->getInput()->getKeyDown(Key::E)?1.0:200.0);
			//rotation
			Quaternion rot;
			if((key==Key::LEFT)-(key==Key::RIGHT)){ 	
				rot.setFromEulero(0,Math::torad((key==Key::LEFT)-(key==Key::RIGHT)),0);		
				camera.setTurn(rot);  
				return;
			}
			if((key==Key::UP)-(key==Key::DOWN)){ 	
				rot.setFromEulero(0,0,0);		
				camera.setTurn(rot);  
				return;
			}
			//
			if(key==Key::W)
				camera.setMove(Vec3(0,0,v));
			else 
			if(key==Key::S)
				camera.setMove(Vec3(0,0,-v));

			else 
			if(key==Key::A)
				camera.setMove(Vec3(v,0,0));
			else 
			if(key==Key::D)
				camera.setMove(Vec3(-v,0,0));

			else
			if(key==Key::Q)
				camera.setMove(Vec3(0,-v,0));
			else 
			if(key==Key::Z)
				camera.setMove(Vec3(0,v,0));			
			

		}
		virtual void onKeyRelease(Key::Keyboard key) {
			//exit event
			if(key==Key::ESCAPE) 
				Application::instance()->exit();		
		}

		virtual void onMouseMove(Vec2 mousePosition) {}
		virtual void onMousePress(Vec2 mousePosition, Key::Mouse button){}
		virtual void onMouseRelease(Vec2 mousePosition, Key::Mouse button){}
		virtual void onMouseScroll(short scrollDelta){}
	};
};
