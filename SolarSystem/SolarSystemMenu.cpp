#include <stdafx.h>
#include <SolarSystemMenu.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
#define DISTANCE_CAMERA_TO_PLANET 7.0f

SolarSystemMenu::SolarSystemMenu(Camera* camera,
                                 PlanetsManager* planets,
                                 const Table& menuConfig)
                                 :camera(camera)
                                 ,cameraCtrl(camera)
                                 ,planets(planets)
                                 ,menu(menuConfig)
                                 ,font(NULL){

    timeCounting=0;
    timeRotation=menuConfig.getFloat("timeRotation",1000.0f)*0.001;
    timeMove=menuConfig.getFloat("timeMove",2500.0f)*0.001;
    turnAngle=0;
    keyAngle=0;
    state.state=FREE_SYSTEM;
    state.planet=NULL;
    if(menuConfig.existsAsType("font", Table::STRING)){
        font=new Font(menuConfig.getTablePath().getDirectory()+"/"+menuConfig.getString("font"));
    }
    //attach inputs...
    isunlock=false;
    unlock();
    cameraCtrl.lock();
    //default values
    float  startAngle=menuConfig.getFloat("startAngle",20);
    float  cameraHigth=menuConfig.getFloat("cameraHigth",0);
    String textPlanet=menuConfig.getString("text");
    Vec2   textPos=menuConfig.getVector2D("textPos",Vec2::ONE*0.5);
    Vec3   textColor=menuConfig.getVector3D("textColor",Vec3(255.0f,255.0f,255.0f));
    float  textTime=menuConfig.getFloat("textTime",1000.0f)*0.001;

                                    
    for(auto planet:*planets){
        Planet *thisPlanet=planet.second;
        //temp data
        float angle=startAngle;
        float camh=cameraHigth;
        String text=textPlanet;
        Vec2   tpos=textPos;
        Vec3   tcolor=textColor;
        float  ttime=textTime;
        //get info
        if(menuConfig.getConstTable("info").existsAsType(planet.first,Table::TABLE)){
            const Table& infoPlanet=menuConfig.getConstTable("info").getConstTable(planet.first);
            angle=infoPlanet.getFloat("startAngle",startAngle);
            camh=infoPlanet.getFloat("cameraHigth",cameraHigth);
            text=infoPlanet.getString("text",textPlanet);
            tpos=infoPlanet.getVector2D("textPos",textPos);
            tcolor=infoPlanet.getVector3D("textColor",textColor);
            ttime=infoPlanet.getFloat("textTime",1000.0f)*0.001;
        }
        //add info
        menu.addOnClick(planet.first,
                        [this,thisPlanet,angle,camh,text,tpos,tcolor,ttime](){
                            if(state.planet!=thisPlanet ){
                                state.state=POIN_TO_PLANET;
                                state.planet=thisPlanet;
                                state.startAngle=angle;
                                state.cameraHigth=camh;
                                state.text=text;
                                state.textPos=tpos;
                                state.textColor=Color(tcolor.r,tcolor.g,tcolor.b,255);
                                state.textTime=ttime;
                                timeCounting=0.0f;
                            }
                        });
    
    }
     //temp data
     float angle=startAngle;
     float camh=cameraHigth;
     String text=textPlanet;
     Vec2   tpos=textPos;
     Vec3   tcolor=textColor;
     float  ttime=textTime;
     //get info
     if(menuConfig.getConstTable("info").existsAsType("sun",Table::TABLE)){
         const Table& infoPlanet=menuConfig.getConstTable("info").getConstTable("sun");
         angle=infoPlanet.getFloat("startAngle",startAngle);
         camh=infoPlanet.getFloat("cameraHigth",cameraHigth);
         text=infoPlanet.getString("text",textPlanet);
         tpos=infoPlanet.getVector2D("textPos",textPos);
         tcolor=infoPlanet.getVector3D("textColor",textColor);
         ttime=infoPlanet.getFloat("textTime",1000.0f)*0.001;
     }
    menu.addOnClick("sun",[this,angle,camh,text,tpos,tcolor,ttime](){
                    if(state.planet!=&this->planets->getSun() ){
                        state.state=POIN_TO_PLANET;
                        state.planet=&this->planets->getSun();
                        state.startAngle=angle;
                        state.cameraHigth=camh;
                        state.text=text;
                        state.textPos=tpos;
                        state.textColor=Color(tcolor.r,tcolor.g,tcolor.b,255);
                        state.textTime=ttime;
                        timeCounting=0.0f;
                      }
                    });
    menu.addOnClick("free",[this](){
        state=State();
        state.state=FREE_SYSTEM;
        state.planet=NULL;
    });
}

SolarSystemMenu::~SolarSystemMenu(){
    if(font)
        delete font;
}

void SolarSystemMenu::pointToPlanet(float dt){
    
    
    //cam pos
    Vec3 plaPos(state.planet->getGlobalMatrix().getTranslation3D());
    Vec3 camPos(camera->getPosition(true));
    //calc rotation points
    Quaternion startrot=camera->getRotation(true);
    Quaternion endrot;
    endrot.setLookRotation(-plaPos + camPos, Vec3(0,1,0));
    //interpolation
    float t=timeCounting/timeRotation;
    camera->setRotation(startrot.slerp(endrot, t));
    //next frame
    timeCounting+=dt;
    //next state
    if(timeCounting>timeRotation){
        state.state=GO_TO_PLANET;
        state.campos=camera->getPosition(true);
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
    float angle=Math::torad(state.startAngle);
    dirrotation.x= dir.x*std::cos(angle)+dir.z*std::sin(angle);
    dirrotation.z=-dir.x*std::sin(angle)+dir.z*std::cos(angle);
    
    Vec3 dirOffset=dirrotation*state.planet->getScale(true)*DISTANCE_CAMERA_TO_PLANET;
    Vec3 endpos=plaPos+dirOffset+sunPos;
    //camera offset
    endpos.y+=state.cameraHigth;
    //http://forums.epicgames.com/threads/892836-Xerp-(non-linear-interpolations)
    //http://wiki.unity3d.com/index.php?title=Mathfx
    #define hermite(t,v) float t=( v * v * (3.0f - 2.0f * v))
    #define coserp(t,v) float t=1.0f - std::cos(v * Math::PI * 0.5f)
    #define sinerp(t,v) float t=std::sin(v * Math::PI * 0.5f)
    #define quad(t,v) float t=(v*2.0)-(v*v)
    //interpolation
    float t=timeCounting/timeMove;
    for(int i=0;i<3;++i)  t=std::sin(t * Math::PI * 0.5f);
    
    
    Vec3 camPos(Math::linear(state.campos, endpos, t)); //lerp quad(quad(quad))
    camera->setPosition(camPos);
    
    //calc rotation points
    Quaternion pointToPlanet;
    pointToPlanet.setLookRotation(-plaPos
                                  +camPos,
                                  Vec3(0,1,0));
    camera->setRotation(pointToPlanet);
    
    //next frame
    timeCounting+=dt;
    //next state
    if(timeCounting>timeMove){
        state.state=ON_PLANET;
        state.campos=camera->getPosition(true);
        timeCounting=0;
        turnAngle=0;
        keyAngle=0;
        state.textColor.a=0;
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
    float angle=Math::torad(state.startAngle+turnAngle);
    dirrotation.x= dir.x*std::cos(angle)+dir.z*std::sin(angle);
    dirrotation.z=-dir.x*std::sin(angle)+dir.z*std::cos(angle);
    turnAngle+=keyAngle*dt;
    
    Vec3 dirOffset=dirrotation*state.planet->getScale(true)*DISTANCE_CAMERA_TO_PLANET;
    Vec3 endpos=plaPos+dirOffset+sunPos;
    //camera offset
    endpos.y+=state.cameraHigth;
    
    //cam pos
    Vec3 camPos(endpos);
    camera->setPosition(camPos);
    
    //calc rotation points
    Quaternion pointToPlanet;
    pointToPlanet.setLookRotation(-plaPos
                                  +camPos,
                                  Vec3(0,1,0));
    camera->setRotation(pointToPlanet);
    //text color
    if(timeCounting<state.textTime){
        timeCounting+=dt;
        float tTime=timeCounting/state.textTime;
        state.textColor.a=(uchar)Math::min(255.0f*tTime,255.0f);
    }

}

void SolarSystemMenu::update(float dt){
    
    cameraCtrl.update();
    camera->update();
    
    switch (state.state) {
        case POIN_TO_PLANET:
            cameraCtrl.lock(); 
            pointToPlanet(dt);
            break;
        case GO_TO_PLANET: 
            goToPlanet(dt); 
            break;
        case ON_PLANET: 
            onPlanet(dt); 
            break;
        case FREE_SYSTEM: 
            cameraCtrl.unlock();
            break;
        default:
            break;
    }
    //update menu
    menu.update(dt);
    
}


void SolarSystemMenu::draw(SolarRender *render){
    menu.draw(render);
    if(state.text.size() && font && state.state==ON_PLANET){
        Vec2 screenPos(Application::instance()->getScreen()->getWidth(),
                       Application::instance()->getScreen()->getHeight());
		Vec2 size=font->sizeText(state.text);
        font->text(screenPos*state.textPos-size*0.5,state.text,state.textColor);
    }
}

