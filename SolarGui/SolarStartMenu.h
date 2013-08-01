#ifndef SOLARSTARTMENU_H
#define SOLARSTARTMENU_H
#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <SolarRender.h>
#include <SolarMenu.h>
#include <Texture.h>
#include <SolarSky.h>

namespace SolarSystem {

	class SolarStartMenu: public MainInstance {
		
		SolarRender    render;
		SolarMenu      menu;
		
		Texture		   *logo;
		Matrix4x4      logoMatrix;

		Texture		   *credits;
		Matrix4x4      creditsMatrix;
		float		   creditsOffset;

		/* background or skybox */
		Texture		   *background;
		Matrix4x4      backgroundMatrix;

		SolarSky	   *skybox;
		Camera		   camera;
		Quaternion	   cameraRot;
		Vec3	       cameraTurn;
		Vec3	       cameraIncTurn;

		bool		   closeApp;

		Table		   config;

	public:

		SolarStartMenu(Table& config):
			MainInstance("Solar System",
						 (int)config.getFloat("width",640),
						 (int)config.getFloat("height",480),
						 (int)config.getFloat("bits",32),
						 (int)config.getFloat("fps",60),
						 config.getString("fullscreen","false")=="true",
						 (int)config.getFloat("MSAA",0))
			,menu(config.getTable("startMenu"))
			,closeApp(true)
			,config(config)
		{
		}


		bool doCloseApp(){
			return closeApp;
		}
		
		void drawTexture(Texture* texture,const Matrix4x4& tranform);

		void start(){
			//init render
			render.init();
			//get table
			DEBUG_ASSERT_MGS_REPLACE(config.existsAsType("startMenu",Table::TABLE),"SolarStartMenu : must to be setted startMenu (TABLE)");
			const Table& startMenu=config.getTable("startMenu");

			DEBUG_ASSERT_MGS_REPLACE(config.existsAsType("startMenu",Table::TABLE),"SolarStartMenu : must to be setted startMenu.background (TABLE)");
			const Table& startBackground=startMenu.getConstTable("background");

			String pathConfig=startMenu.getTablePath().getDirectory()+"/";
			//load
			Utility::Path logopath=pathConfig+startMenu.getString("logo","logo.png");
			logo=new Texture(logopath);		

			Utility::Path creditspath=pathConfig+startMenu.getString("credits","credits.png");
			credits=new Texture(creditspath);					

			creditsOffset=startMenu.getFloat("creditsOffset",0);
			//query
			Screen *screen=Application::instance()->getScreen();
			Vec2 windowSize(screen->getWidth(),screen->getHeight());
			Vec2 windowCenter=windowSize*0.5;
			//init logo matrix
			Vec2 logoscale(512,512);
			logoMatrix.setScale(logoscale);
			logoMatrix[12]=windowCenter.x;
			logoMatrix[13]=windowCenter.y;
			//init crediti matrix
			Vec2 creditsscale(256,256);
			creditsMatrix.setScale(creditsscale);
			creditsMatrix[12]=windowCenter.x;
			creditsMatrix[13]=windowCenter.y+creditsOffset;


			if(startBackground.existsAsType("skybox",Table::TABLE)){
				//load skybox
				this->skybox=new SolarSky(&render,startBackground.getConstTable("skybox"));			
				//enable camera
				float wfactor=(float)Application::instance()->getScreen()->getHeight()/Application::instance()->getScreen()->getWidth();
				camera.setPerspective(-0.5, 0.5,-0.5*wfactor,0.5*wfactor, 1.0f,100000.0f);
				//roll to 0
				cameraTurn=startBackground.getVector3D("angleStart",Vec3(0.0,0.0,0.0));
				cameraTurn.x=Math::torad(cameraTurn.x);
				cameraTurn.y=Math::torad(cameraTurn.y);
				cameraTurn.z=Math::torad(cameraTurn.z);

				cameraIncTurn=startBackground.getVector3D("angleTurn",Vec3(1.0,0.0,0.0));
				cameraIncTurn.x=Math::torad(cameraIncTurn.x);
				cameraIncTurn.y=Math::torad(cameraIncTurn.y);
				cameraIncTurn.z=Math::torad(cameraIncTurn.z);

			}
			else{
				Utility::Path backgroundpath=startBackground.getTablePath().getDirectory()+"/"+
											 startBackground.getString("image","background.png");
				background=new Texture(backgroundpath);
				//init background matrix
				float hsize=((float)(background->getHeight()*screen->getWidth()))/background->getWidth();
				Vec2 backgroundscale(screen->getWidth(),hsize);
				backgroundMatrix.setScale(backgroundscale);
				backgroundMatrix[12]=windowCenter.x;
				backgroundMatrix[13]=windowCenter.y;
			}

			//events
			menu.addOnClick("start",[this](){
				this->closeApp=false;
				Application::instance()->exit();
			});
			menu.addOnClick("exit",[this](){
				this->closeApp=true;
				Application::instance()->exit();
			});
		}
		void run(float dt){	
			//clear
			render.setClearColor(Vec4(Vec3::ZERO,1.0f));
			//update menu
			menu.update(dt);
			//skybox effect
			if(skybox){
				cameraTurn+=cameraIncTurn*dt;
				cameraRot.setFromEulero(cameraTurn.x,
										cameraTurn.y,
										cameraTurn.z);
				camera.setRotation(cameraRot);
				////////////////////////////////////////////////////////////////
				camera.update();
				////////////////////////////////////////////////////////////////
				skybox->draw(camera);
			}
			else{
				//draw background
				drawTexture(background,backgroundMatrix);
			}
			//draw logo
			drawTexture(logo,logoMatrix);
			//draw credits
			drawTexture(credits,creditsMatrix);
			//draw gui
			menu.draw(&render);
		}
		void end(){
			delete logo;
			delete credits;
			delete background;
		}

	};

};

#endif