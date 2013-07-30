#ifndef SOLARSTARTMENU_H
#define SOLARSTARTMENU_H
#include <Config.h>
#include <Application.h>
#include <MainInstance.h>
#include <SolarRender.h>
#include <SolarMenu.h>
#include <Texture.h>

namespace SolarSystem {

	class SolarStartMenu: public MainInstance {
		
		SolarRender    render;
		SolarMenu      menu;
		
		Texture		   *logo;
		Matrix4x4      logoMatrix;

		Texture		   *credits;
		Matrix4x4      creditsMatrix;
		float		   creditsOffset;

		Texture		   *background;
		Matrix4x4      backgroundMatrix;

		bool		   closeApp;

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
		{
			
			Utility::Path logopath=config.getTable("startMenu").getTablePath().getDirectory()+"/"+
								   config.getTable("startMenu").getString("logo","logo.png");
			logo=new Texture(logopath);
			
			Utility::Path creditspath=config.getTable("startMenu").getTablePath().getDirectory()+"/"+
								      config.getTable("startMenu").getString("credits","credits.png");
			credits=new Texture(creditspath);			

			Utility::Path backgroundpath=config.getTable("startMenu").getTablePath().getDirectory()+"/"+
								         config.getTable("startMenu").getString("background","background.png");
			background=new Texture(backgroundpath);

			creditsOffset=config.getTable("startMenu").getFloat("creditsOffset",0);
		}


		bool doCloseApp(){
			return closeApp;
		}
		
		void drawTexture(Texture* texture,const Matrix4x4& tranform);

		void start(){
			//init render
			render.init();
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
			//init background matrix
			float hsize=((float)(background->getHeight()*screen->getWidth()))/background->getWidth();
			Vec2 backgroundscale(screen->getWidth(),hsize);
			backgroundMatrix.setScale(backgroundscale);
			backgroundMatrix[12]=windowCenter.x;
			backgroundMatrix[13]=windowCenter.y;
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
			//draw background
			drawTexture(background,backgroundMatrix);
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