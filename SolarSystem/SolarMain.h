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
		Planet earth;
		Planet mars;
		Planet sun;
		Camera camera;
		Object obj;
		float days;

	public:
		SolarMain():
			MainInstance("Solar System",786,786,32,60)
			,earth("img/earth.png")
			,mars("img/mars.png")
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
		camera.setPerspective(45,0.1f,10000.0f);
		camera.setPosition(Vec3(0,-120,-500));
		Quaternion quad;
		quad.setFromEulero(Math::torad(-20),0,0);
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
		/////////////////////////////////////////////
		//set info earth
		days=0;
		earth.setPlanetInfo(Vec2(147,152),360);
		earth.setData(days);
		earth.setScale(Vec3(12,12,12)/2);
		//mars
		mars.setPlanetInfo(Vec2(206,249),320);
		mars.setData(days);
		mars.setScale(Vec3(6,6,6)/2);
		//sun
		sun.setPlanetInfo(Vec2(0,0),1);
		sun.setData(days);
		sun.setScale(Vec3(150,150,150)/2);

		}
		virtual void run(float dt){		
			//clear
			glClearColor(0.25f, 0.5f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			//update camera
			camera.update();
			days+=1.f;

			earth.setData(days);
			earth.draw(camera);

			mars.setData(days);
			mars.draw(camera);	

			sun.setData(days);
			sun.draw(camera);			
		}
		virtual void end(){
		}
		
		virtual void onKeyPress(Key::Keyboard key){
			//rotation
			Quaternion rot;
			rot.setFromEulero(0,Math::torad((key==Key::LEFT)-(key==Key::RIGHT)),0);		
			if((key==Key::LEFT)-(key==Key::RIGHT)){ 	
				camera.setTurn(rot);  
				return;
			}
			//
			if(key==Key::W)
				camera.setTranslation(Vec3(0,0,.1));
			else 
			if(key==Key::S)
				camera.setTranslation(Vec3(0,0,-.1));

			else 
			if(key==Key::A)
				camera.setTranslation(Vec3(.1,0,0));
			else 
			if(key==Key::D)
				camera.setTranslation(Vec3(-.1,0,0));

			else
			if(key==Key::Q)
				camera.setTranslation(Vec3(0,-.1,0));
			else 
			if(key==Key::Z)
				camera.setTranslation(Vec3(0,.1,0));			
			

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
