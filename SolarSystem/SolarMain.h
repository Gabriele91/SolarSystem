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

	public:
		SolarMain():
			MainInstance("Solar System",1920,1080,32,60,false)
			,system(&camera,&render)
		{
		
		}
		virtual void start(){
		//input
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
		Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
		//init render
		render.init();
		//setup camera
		camera.setPerspective(95,1.0f,100000.0f);
		Quaternion quad;
		//quad.setFromEulero(Math::torad(-90),0,0);
		//camera.setPosition(Vec3(0,-10000,0));
		quad.setFromEulero(Math::torad(0),0,0);
		camera.setPosition(Vec3(0,0,-9500));
		camera.setRotation(quad);		
		//scale factor
		days=900;
		system.setScaleElipses(0.1);
		system.setScalePlanets(0.2);
		system.setScaleSun(2);
		/////////////////////////////////////////////	
		//Elipses are in MKm
		//SUN is in KKm
		//PLANETS is in KKm
		//sun
		system.addSun("img/sun.png",
					  Vec3(1500,1500,1500),
					  25.379995);
		//mercury
		system.addPlanet("img/mercury.png",
						 Vec2(69,46),
						 Vec3(4,4,4),
						 87.97,
					     58.6462);
		//earth
		system.addPlanet("img/earth.png",
						 Vec2(147,152),
						 Vec3(12,12,12),
						 360,
						 0.99726968);
		//mars
		system.addPlanet("img/mars.png",
						 Vec2(206,249),
						 Vec3(6,6,6),
						 320,
						 1.02595675);
		//jupiter
		system.addPlanet("img/jupiter.png",
						 Vec2(740,810),
						 Vec3(70,70,70),
						 4332.82f,
						 0.41007);

		}
		virtual void run(float dt){				
			days+=60.0f*dt;
			//clear screen
			render.setClearColor(Vec4(Vec3::ZERO,1.0f));
			//font buffer
			system.setData(days);
			system.draw();
			//get errors...
			CHECK_GPU_ERRORS();
		}
		virtual void end(){
		}
		
		virtual void onKeyPress(Key::Keyboard key){
			static const float v=100.0;
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
