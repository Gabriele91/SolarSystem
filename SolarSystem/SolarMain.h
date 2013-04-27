#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <Math2D.h>
#include <Camera.h>
#include <Texture.h>
#include <Shader.h>
#include <Planet.h>
#include <PlanetsManager.h>

namespace SolarSystem {

	class SolarMain: public MainInstance,
							Input::KeyboardHandler,
							Input::MouseHandler{
		
		PlanetsManager system;
		Shader blackMesh;
		Shader godRays;
		RenderTexture blackTexture;
		RenderTexture colorTexture;
		Camera camera;
		Object obj;
		float days;

	public:
		SolarMain():
			MainInstance("Solar System",1920,1080,32,60,false)
			,system(&camera)
			,blackMesh("shader/blackMesh.vs","shader/blackMesh.ps")
			,godRays("shader/godRays.vs","shader/godRays.ps")
			,blackTexture(1920,1080)
			,colorTexture(1920,1080)
		{
		
		}
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
		quad.setFromEulero(Math::torad(0),0,0);
		camera.setPosition(Vec3(0,0,-9900));
		//quad.setFromEulero(Math::torad(-90),0,0);
		//camera.setPosition(Vec3(0,-10000,0));
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
		days=900;
		system.setScaleElipses(0.1);
		system.setScalePlanets(0.2);
		system.setScaleSun(2);
		/////////////////////////////////////////////	
		//Elipses are in MKm
		//SUN is in KKm
		//PLANETS is in KKm
		//sun
		system.addSun("img/sun.png",Vec3(1500,1500,1500));
		//mercury
		system.addPlanet("img/mercury.png",
						 Vec2(69,46),
						 Vec3(4,4,4),
						 87.97);
		//earth
		system.addPlanet("img/earth.png",
						 Vec2(147,152),
						 Vec3(12,12,12),
						 360);
		//mars
		system.addPlanet("img/mars.png",
						 Vec2(206,249),
						 Vec3(6,6,6),
						 320);
		//jupiter
		system.addPlanet("img/jupiter.png",
						 Vec2(740,810),
						 Vec3(70,70,70),
						 4332.82f);

		}
		virtual void run(float dt){		
			

			days+=20.f*dt;
			system.setData(days);

			//offscreen draw
			blackTexture.enableRender();				
				glViewport(0, 0, blackTexture.getWidth(),
								 blackTexture.getHeight());
				//clear
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				//
				camera.update();
				system.drawSun();
				blackMesh.bind();
					system.drawPlanets();
				blackMesh.unbind();
			blackTexture.disableRender();	


			//font buffer
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, Application::instance()->getScreen()->getWidth(),
							 Application::instance()->getScreen()->getHeight());
			//draw
			system.draw();

			//draw vbo
			godRays.bind();
			float uniformExposure = 0.0034f;
			float uniformDecay = 1.0f;
			float uniformDensity = 1.f;
			float uniformWeight = 3.65f;
			godRays.uniformFloat("exposure",uniformExposure);
			godRays.uniformFloat("decay",uniformDecay);
			godRays.uniformFloat("density",uniformDensity);
			godRays.uniformFloat("weight",uniformWeight);
			godRays.uniformVector2D("lightPositionOnScreen",camera.getPointIn3DSpace(Vec3::ZERO));
			godRays.uniformInt("myTexture",0);
			
			glEnable( GL_BLEND );   
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			blackTexture.draw();
			godRays.unbind(); 

			//default status for blending    
			glEnable( GL_BLEND );   
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

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
