#include <stdafx.h>
#include <SolarSystemMenu.h>

///////////////////////
using namespace SolarSystem;
///////////////////////


SolarSystemMenu::SolarSystemMenu(Camera* camera,
                                 PlanetsManager* planets,
                                 const Table& menuConfig)
                                 :camera(camera)
                                 ,planets(planets)
                                 ,menu(menuConfig){

    timeCounting=0;
    timeRotation=menuConfig.getFloat("timeRotation",1000)*0.001;
    timeMove=menuConfig.getFloat("timeMove",2500)*0.001;
    startAngle=menuConfig.getFloat("startAngle",20);
    turnAngle=0;
    keyAngle=0;
    state.state=FREE_SYSTEM;
    state.planet=NULL;
                                     
    Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
    isunlock=true;
                                     
    for(auto planet:*planets){
        Planet *thisPlanet=planet.second;
        menu.addOnClick(planet.first,
                        [this,thisPlanet](){
                            if(state.planet!=thisPlanet ){
                                state.state=POIN_TO_PLANET;
                                state.planet=thisPlanet;
                                timeCounting=0.0f;
                            }
                        });
    
    }
    menu.addOnClick("sun",[this](){
                    if(state.planet!=&this->planets->getSun() ){
                         state.state=POIN_TO_PLANET;
                         state.planet=&this->planets->getSun();
                         timeCounting=0.0f;
                      }
                    });
}

void SolarSystemMenu::pointToPlanet(float dt){
    
    
    //cam pos
    Vec3 plaPos(state.planet->getGlobalMatrix().getTranslation3D());
    Vec3 camPos(camera->getPosition(true));
    //calc rotation points
    float endP,endY,endR;
    Quaternion endrot;
    endrot.setLookRotation(-plaPos
                           -camPos,
                           Vec3(0,1,0));
    
    float startP,startY,startR;
    Quaternion startrot=camera->getRotation(true);
    //get eulero rotarion
    endrot.getEulero(endP,endY,endR);
    startrot.getEulero(startP,startY,startR);
    //interpolation
    float t=timeCounting/timeRotation;
    Quaternion intrp;
    intrp.setFromEulero(Math::lerp(startP, endP, t),
                        Math::lerp(startY, endY, t),
                        Math::lerp(startR, endR, t));
    //set rotation
    camera->setRotation(intrp);
    //next frame
    timeCounting+=dt;
    //next state
    if(timeCounting>timeRotation){
        state.state=GO_TO_PLANET;
        timeCounting=0;
    }
}
void SolarSystemMenu::goToPlanet(float dt){
    
    //calc point pos
    Vec3 plaPos(state.planet->getGlobalMatrix().getTranslation3D());
    Vec3 sunPos=planets->getSun().getPosition(true);
    
    Vec3 dir=plaPos-sunPos;
    float lengdir=dir.length();
    if(lengdir>0.0) dir/=lengdir; else dir=Vec3::ONE;
    
    Vec3 dirrotation;
    float angle=Math::torad(startAngle);
    dirrotation.x= dir.x*std::cos(angle)+dir.z*std::sin(angle);
    dirrotation.z=-dir.x*std::sin(angle)+dir.z*std::cos(angle);
    
    Vec3 dirOffset=dirrotation*state.planet->getScale(true)*4;
    Vec3 endpos=plaPos+dirOffset+sunPos;
    
    
    //interpolation
    float t=timeCounting/timeMove;
    Vec3 camPos(Math::lerp(camera->getPosition(true), -endpos, t));
    camera->setPosition(camPos);
    
    //calc rotation points
    Quaternion pointToPlanet;
    pointToPlanet.setLookRotation(-plaPos
                                  -camPos,
                                  Vec3(0,1,0));
    camera->setRotation(pointToPlanet);
    
    //next frame
    timeCounting+=dt;
    //next state
    if(timeCounting>timeMove){
        state.state=ON_PLANET;
        timeCounting=0;
        turnAngle=0;
        keyAngle=0;
    }
    
}
void SolarSystemMenu::onPlanet(float dt){
    //calc point pos
    Vec3 plaPos(state.planet->getGlobalMatrix().getTranslation3D());
    Vec3 sunPos=planets->getSun().getPosition(true);
    
    Vec3 dir=plaPos-sunPos;
    float lengdir=dir.length();
    if(lengdir>0.0) dir/=lengdir; else dir=Vec3::ONE;
    
    Vec3 dirrotation;
    float angle=Math::torad(startAngle+turnAngle);
    dirrotation.x= dir.x*std::cos(angle)+dir.z*std::sin(angle);
    dirrotation.z=-dir.x*std::sin(angle)+dir.z*std::cos(angle);
    turnAngle+=keyAngle*dt;
    
    Vec3 dirOffset=dirrotation*state.planet->getScale(true)*4;
    Vec3 endpos=plaPos+dirOffset+sunPos;
    
    //cam pos
    Vec3 camPos(-endpos);
    camera->setPosition(camPos);
    
    //calc rotation points
    Quaternion pointToPlanet;
    pointToPlanet.setLookRotation(-plaPos
                                  -camPos,
                                  Vec3(0,1,0));
    camera->setRotation(pointToPlanet);

}

void SolarSystemMenu::update(float dt){
    
    
    camera->update();
    
    switch (state.state) {
        case POIN_TO_PLANET: pointToPlanet(dt); break;
        case GO_TO_PLANET: goToPlanet(dt); break;
        case ON_PLANET: onPlanet(dt); break;
        case FREE_SYSTEM: break;
        default:
            break;
    }
    //update menu
    menu.update(dt);
    
}


void SolarSystemMenu::draw(SolarRender *render){
    menu.draw(render);
}

