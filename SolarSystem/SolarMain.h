#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <Math2D.h>
#include <Camera.h>
#include <Texture.h>
#include <Planet.h>

namespace SolarSystem {

	class SolarMain: public MainInstance,
							Input::KeyboardHandler,
							Input::MouseHandler{
		Planet jupiter;
		Planet mars;
		Planet earth;
		Planet mercury;
		Planet sun;
		Camera camera;
		Object obj;
		float days;

	public:
		SolarMain():
			MainInstance("Solar System",1920,1080,32,60,true)
			,mercury("img/mercury.png")
			,earth("img/earth.png")
			,mars("img/mars.png")
			,jupiter("img/jupiter.png")
			,sun("img/sun.png"){}
		virtual void start(){
		//input
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
		Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
		/////////////////////////////////////////////
		//OPENGL
		//view port
		glViewport(0, 0, Application::instance()->getScreen()->getWidth(),
						 Application::instance()->getScreen()->getHeight());
		//enable culling
		glEnable( GL_CULL_FACE );
		glCullFace( GL_FRONT );
		//enable z buffer
		glEnable(GL_DEPTH_TEST);
		//set projection matrix
		camera.setPerspective(95,1.0f,100000.0f);
		Quaternion quad;
		//quad.setFromEulero(Math::torad(-15),0,0);
		//camera.setPosition(Vec3(0,-80,-500));
		quad.setFromEulero(Math::torad(-90),0,0);
		camera.setPosition(Vec3(0,-10000,0));
		camera.setRotation(quad);
		//enable texturing	
		glEnable( GL_TEXTURE_2D );
		//enable state	
		//always active!
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);       
		//default status for blending    
		glEnable(GL_ALPHA_TEST);
        glEnable( GL_BLEND );   
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		//scale factor
		const float scale=1.0/0.1;
		const float scalePlanet=1.0/0.3;
		const float scaleSun=1.0/2.0;
		days=0;
		/////////////////////////////////////////////	
		//Elipses are in MKm
		//SUN is in KKm
		//PLANETS is in KKm
		//sun
		sun.setPlanetInfo(Vec2(0,0)*scale,1);
		sun.setData(days);
		sun.setScale(Vec3(1500,1500,1500)*scaleSun*0.5);
		//mercury
		mercury.setPlanetInfo(Vec2(69,46)*scale,87.97);
		mercury.setData(days);
		mercury.setScale(Vec3(4,4,4)*scalePlanet*0.5);
		//earth
		earth.setPlanetInfo(Vec2(147,152)*scale,360);
		earth.setData(days);
		earth.setScale(Vec3(12,12,12)*scalePlanet*0.5);
		//mars
		mars.setPlanetInfo(Vec2(206,249)*scale,320);
		mars.setData(days);
		mars.setScale(Vec3(6,6,6)*scalePlanet*0.5);
		//jupiter
		jupiter.setPlanetInfo(Vec2(740,810)*scale,4332.82f);
		jupiter.setData(days);
		jupiter.setScale(Vec3(70,70,70)*scalePlanet*0.5);

		}
		virtual void run(float dt){		
			//clear
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			//update camera
			camera.update();
			days+=2.f;

			mercury.setData(days);
			mercury.draw(camera);

			earth.setData(days);
			earth.draw(camera);	

			mars.setData(days);
			mars.draw(camera);	

			jupiter.setData(days);
			jupiter.draw(camera);
			
			sun.setData(days);
			sun.draw(camera);			
		}
		virtual void end(){
		}
		
		virtual void onKeyPress(Key::Keyboard key){
			static const float v=10.0;
			//rotation
			Quaternion rot;
			if((key==Key::LEFT)-(key==Key::RIGHT)){ 	
				rot.setFromEulero(0,Math::torad((key==Key::LEFT)-(key==Key::RIGHT)),0);		
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
