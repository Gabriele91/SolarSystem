#include <stdafx.h>
#include <SolarFly.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

SolarFly::SolarFly(Camera *camera):camera(camera)
		                          ,turnVel(0.1,0.1)
		                          ,movVel(10.0,10.0,10.0)
                                  ,isLocked(false){
    //init game pads
    pads.init();
    pads.addHandler(0,this);
}
SolarFly::~SolarFly()
{
    //disable pads
    pads.end();
}
///////////////////////
float stickFix(float value,float min)
{
    bool neg=value<0.0;
    float av=std::abs(value);
    float nv=Math::saturate<float>(av-min)*(neg ? 1.0f : -1.0f);
    return (1/min)*nv;
}

void SolarFly::update()
{
    //update events
    pads.update();
    //loop events
    auto* info=pads.getInfo(0);
    // ... xbox 360 pad.
    if(info)
    {
        //x move
        float velRX = stickFix(info->axisStates[2],0.25f)*-1.0;
		camera->setMove(Vec3(movVel.y*velRX,0,0));
        //y move
        float velRY = stickFix(info->axisStates[3],0.25f)*-1.0;
		camera->setMove(Vec3(0,movVel.y*velRY,0));
        //z move
        float velR  = (info->axisStates[5]+1.0f)*0.5f;
        float velL  = (info->axisStates[4]+1.0f)*0.5f;
        float velZ  = velR-velL;
		camera->setMove(Vec3(0,0,-movVel.z*velZ));
        //camera turn
        float stickLY = stickFix(info->axisStates[0],0.25f);
        float stickLX = stickFix(info->axisStates[1],0.25f)*-1.0f;
	    Quaternion rot;
        rot.setFromEulero(0,Math::torad(stickLY)*turnVel.y,0);
        camera->setTurn(rot);
		rot.setFromEulero(Math::torad(stickLX)*turnVel.x,0,0);
        camera->setTurn(rot);
    }
}
///////////////////////
void SolarFly::onKeyDown(Key::Keyboard key){
    //lock?
    if(isLocked) return;
	//rotation
	Quaternion rot;
	if((key==Key::LEFT)-(key==Key::RIGHT)){
		rot.setFromEulero(0,Math::torad((key==Key::LEFT)-(key==Key::RIGHT))*turnVel.y,0);
		camera->setTurn(rot);
		return;
	}
	if((key==Key::UP)-(key==Key::DOWN)){
		rot.setFromEulero(Math::torad((key==Key::UP)-(key==Key::DOWN))*turnVel.x,0,0);
		camera->setTurn(rot);
		return;
	}
	//
	if(key==Key::W)
		camera->setMove(Vec3(0,0,-movVel.z));
	else
	if(key==Key::S)
		camera->setMove(Vec3(0,0, movVel.z));

	else
	if(key==Key::A)
		camera->setMove(Vec3(-movVel.x,0,0));
	else
	if(key==Key::D)
		camera->setMove(Vec3(movVel.x,0,0));

	else
	if(key==Key::Q)
		camera->setMove(Vec3(0,movVel.y,0));
	else
	if(key==Key::Z)
		camera->setMove(Vec3(0,-movVel.y,0));

    else
	if(key==Key::N1)
		setMoveVelocity(Vec3(1,1,1));
	else 
    if(key==Key::N2)
		setMoveVelocity(Vec3(10.0,10.0,10.0));
	else 
    if(key==Key::N3)
		setMoveVelocity(Vec3(100.0,100.0,100.0));
	else 
    if(key==Key::N4)
		setMoveVelocity(Vec3(1000.0,1000.0,1000.0));
}
void SolarFly::onMouseDown(Vec2 mousePosition, Key::Mouse button){
    //lock?
    if(isLocked) return;
    //
	if(button==Key::BUTTON_MIDDLE){
				
		Vec2 center(Application::instance()->getScreen()->getWidth()*0.5,
					Application::instance()->getScreen()->getHeight()*0.5);
		//get direction and intensity
		Vec2 dir=Application::instance()->getInput()->getMouse()-center;
		float leng=dir.length();
        if(leng>0.0f){
            dir/=leng;//normalize

            Quaternion rot;
            if(dir.x>0) rot.setFromEulero(0,Math::torad(-1)*leng*turnVel.y,0);
            if(dir.x<0) rot.setFromEulero(0,Math::torad(1)*leng*turnVel.y,0);
            camera->setTurn(rot);
            if(dir.y>0) rot.setFromEulero(Math::torad(-1)*leng*turnVel.x,0,0);
            if(dir.y<0) rot.setFromEulero(Math::torad(1)*leng*turnVel.x,0,0);
            camera->setTurn(rot);
        }
		Application::instance()->getScreen()->setPositionCursor(center);
	}
}
//pad
void SolarFly::onButtonDown(uint button, double timestamp)
{
    //lock?
    if(isLocked) return;
    //else
    if(button==8)
		setMoveVelocity(getMoveVelocity()*Vec3(.1,.1,.1));
    else
    if(button==9)
		setMoveVelocity(getMoveVelocity()*Vec3(10,10,10));
}
void SolarFly::onButtonUp(uint button,  double timestamp)
{
    //lock?
    if(isLocked) return;
    //else
}
void SolarFly::onAxisMove(uint axisID, float value, float lastValue, double timestamp)
{
    //lock?
    if(isLocked) return;
    //else
}
//lock / unloack
void SolarFly::lock(){
    isLocked=true; 
}
void SolarFly::unlock(){
    if(isLocked)
    {
        Vec2 center(Application::instance()->getScreen()->getWidth()*0.5,
			        Application::instance()->getScreen()->getHeight()*0.5);
        Application::instance()->getScreen()->setPositionCursor(center);
    }
    isLocked=false; 
}