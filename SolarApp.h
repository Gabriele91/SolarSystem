//
//  SolarApp.h
//  SolarSystem
//
//  Created by Gabriele Di Bari on 04/08/13.
//
//
#ifndef SOLARAPP_H
#define SOLARAPP_H

#include <Config.h>
#include <Application.h>
#include <SolarMain.h>
#include <SolarStartMenu.h>
#include <SolarSound.h>

namespace SolarSystem {
    
	class SolarApp: public MainInstance{
        
        Table *configureTable;
        SolarStartMenu *solarmenu;
        SolarMain *solarmain;
        SolarSound *solarMusic;
        
		//loading screen
		SolarRender    render;
		Texture		   *loadingScreen;
		Matrix4x4      loadingScreenMatrix;
		void drawLoading();
		void loading(){
			render.setClearColor(Vec4(Vec3::ZERO,1.0));
			drawLoading();
			getScreen()->swap();
		}

        enum APPSTATE{
            MENU_STATE,
            SOLAR_STATE
        }state;
    
    public:
        
		SolarApp(Table& config):
        MainInstance("Solar System",
                     (int)config.getFloat("width",640),
                     (int)config.getFloat("height",480),
                     (int)config.getFloat("bits",32),
                     (int)config.getFloat("fps",60),
                     config.getString("fullscreen","false")=="true",
                     (int)config.getFloat("MSAA",0))
                     ,solarmenu(NULL)
                     ,solarmain(NULL)
                     ,solarMusic(NULL)
        {
             configureTable=&config;
             state=MENU_STATE;
			 //loading setting
			 render.init();
			 //load			 
			String pathConfig=config.getTablePath().getDirectory()+"/";
			Utility::Path logopath=pathConfig+config.getString("loadingScreen","loadingScreen.png");
		    loadingScreen=new Texture(logopath);	
			//init loading screen matrix
			Vec2 windowSize(getScreen()->getWidth(),getScreen()->getHeight());
			Vec2 windowCenter=windowSize*0.5;
			Vec2 logoscale=config.getVector2D("loadingScreenSize",Vec2(512,512));
			loadingScreenMatrix.setScale(logoscale);
			loadingScreenMatrix[12]=windowCenter.x;
			loadingScreenMatrix[13]=windowCenter.y+config.getFloat("loadingScreenOffset",0.0);
        }
        
        virtual void start(){
            //load sound
            solarMusic=new SolarSound(*configureTable);
            //play sound
            solarMusic->loopMenu();
            //load menu
			loading();
            solarmenu=new SolarStartMenu(*configureTable);
            solarmenu->start();
        }
        virtual void run(float dt){
            
            
            if(state==MENU_STATE)
                solarmenu->run(dt);
            else
                solarmain->run(dt);
            
            //event menu
            if(solarmenu->doCloseApp())
               Application::instance()->exit();
            
            if(solarmenu->doPlayApp()){
                state=SOLAR_STATE;
                solarMusic->loopSystem();
                if(!solarmain){
					loading();
                    solarmain=new SolarMain(*configureTable);
                    solarmain->start();
                }
				else
					solarmain->enableEvents();
            }
            //event solar
            if(state==SOLAR_STATE){
                if(solarmain->doReturnToMenu()){
                    state=MENU_STATE;
                    solarMusic->loopMenu();
					solarmain->disableEvents();
                }
            }
            
        }
        virtual void end(){
            if(solarMusic)
                delete solarMusic;
            if(solarmenu){
                solarmenu->end();
                delete solarmenu;
            }
            if(solarmain){
                solarmain->end();
                delete solarmain;
            }
        }
    
    
    };

}
#endif