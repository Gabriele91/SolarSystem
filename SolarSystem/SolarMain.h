#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <Camera.h>

namespace SolarSystem {
	class SolarMain: public MainInstance,
							Input::KeyboardHandler,
							Input::MouseHandler{

		Camera camera;
		Object obj;
		void drawSphere(double r, int lats, int longs) {
	        int i, j;
	        for(i = 0; i <= lats; i++) {
	            double lat0 = Math::PI * (-0.5 + (double) (i - 1) / lats);
	           double z0  = sin(lat0);
	           double zr0 =  cos(lat0);
    
	           double lat1 = Math::PI * (-0.5 + (double) i / lats);
	           double z1 = sin(lat1);
	           double zr1 = cos(lat1);
    
	           glBegin(GL_QUAD_STRIP);
	           for(j = 0; j <= longs; j++) {
	               double lng = 2 * Math::PI * (double) (j - 1) / longs;
	               double x = cos(lng);
	               double y = sin(lng);
    
	               glNormal3f(x * zr0, y * zr0, z0);
	               glVertex3f(x * zr0, y * zr0, z0);
	               glNormal3f(x * zr1, y * zr1, z1);
	               glVertex3f(x * zr1, y * zr1, z1);
	           }
	           glEnd();
	       }
	   }

	public:
		SolarMain():MainInstance("Solar System",1280,786){}
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
		camera.setPerspective(45,0.1f,1000.0f);
		//enable state
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
		/////////////////////////////////////////////
		}
		virtual void run(float dt){		
			//clear
			glClearColor(0.25f, 0.5f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			//update camera
			camera.update();
			//
			int spheresTotal=0;
			int spheresDrawn=0;
			//
			int xcount=10;
			int zcount=10;
			//
			for (int i = -xcount; i < xcount; i+=4) 
			for(int k =  -zcount; k < zcount; k+=4) {
				glColor3f(float(i+xcount)/(xcount*2.0f),float(k+zcount)/(zcount*2.0f),0);
				spheresTotal++;
				Vec3 pos(i,0,k);
				obj.setPosition(pos);
				if (Application::instance()->getInput()->getMouseDown(Key::BUTTON_RIGHT)
					||(camera.sphereInFrustum(pos,5) != Camera::OUTSIDE)) {
					glLoadMatrixf(camera.getGlobalMatrix().mul(obj.getGlobalMatrix()));
					drawSphere(5,30,30);
					spheresDrawn++;
				}
			};
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
