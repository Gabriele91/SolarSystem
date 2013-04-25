#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <Math2D.h>
#include <Camera.h>
#include <Texture.h>

namespace SolarSystem {

	class SolidSphere{
	protected:
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> texcoords;
		std::vector<GLushort> indices;

	public:

		SolidSphere(float radius, unsigned int rings, unsigned int sectors){
			float const R = 1./(float)(rings-1);
			float const S = 1./(float)(sectors-1);
			int r, s;

			vertices.resize(rings * sectors * 3);
			normals.resize(rings * sectors * 3);
			texcoords.resize(rings * sectors * 2);
			std::vector<GLfloat>::iterator v = vertices.begin();
			std::vector<GLfloat>::iterator n = normals.begin();
			std::vector<GLfloat>::iterator t = texcoords.begin();
			for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
					float const y = sin( -0.5*Math::PI + Math::PI * r * R );
					float const x = cos(  2*Math::PI * s * S) * sin( Math::PI * r * R );
					float const z = sin(  2*Math::PI * s * S) * sin( Math::PI * r * R );

					*t++ = s*S;
					*t++ = r*R;

					*v++ = x * radius;
					*v++ = y * radius;
					*v++ = z * radius;

					*n++ = x;
					*n++ = y;
					*n++ = z;
			}
			
			indices.resize(rings * sectors * 6);
			int i = -1;
			for(int r = 0; r < rings-1; r++) {
				for(int s = 0; s < sectors-1; s++) {
					//1 2 3
					indices[++i] = r * sectors + s;
					indices[++i] = r * sectors + (s+1);
					indices[++i] = (r+1) * sectors + (s+1);
					//1 3 4
					indices[++i] = r * sectors + s;
					indices[++i] = (r+1) * sectors + (s+1);
					indices[++i] = (r+1) * sectors + s;
				}
			}
		}
		void draw(){
			glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
			glNormalPointer(GL_FLOAT, 0, &normals[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
		}
	};

	class SolarMain: public MainInstance,
							Input::KeyboardHandler,
							Input::MouseHandler{
		SolidSphere sphere;
		Texture tex;
		Camera camera;
		Object obj;

	public:
		SolarMain():
			MainInstance("Solar System",786,786,32,30)
			,tex("img/earth.png")
			,sphere(1,30,30){}
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
		}
		virtual void run(float dt){		
			//clear
			glClearColor(0.25f, 0.5f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			//update camera
			camera.update();
			tex.bind();
			//
			int spheresTotal=0;
			int spheresDrawn=0;
			//
			int xcount=230;
			int zcount=230;
			//
			for (int i = -xcount; i < xcount; i+=4) 
			for(int k =  -zcount; k < zcount; k+=4) {
				spheresTotal++;
				Vec3 pos(i,0,k);
				obj.setPosition(pos);
				obj.setScale(Vec3(2,2,2));
				if (Application::instance()->getInput()->getMouseDown(Key::BUTTON_RIGHT)
					||(camera.sphereInFrustum(pos,2) != Camera::OUTSIDE)) {
					glLoadMatrixf(camera.getGlobalMatrix().mul(obj.getGlobalMatrix()));
					sphere.draw();
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
