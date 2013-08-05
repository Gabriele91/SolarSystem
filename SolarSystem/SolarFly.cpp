#include <stdafx.h>
#include <SolarFly.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
void SolarFly::onKeyDown(Key::Keyboard key){
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
		camera->setMove(Vec3(0,0,movVel.z));
	else
	if(key==Key::S)
		camera->setMove(Vec3(0,0,-movVel.z));

	else
	if(key==Key::A)
		camera->setMove(Vec3(movVel.x,0,0));
	else
	if(key==Key::D)
		camera->setMove(Vec3(-movVel.x,0,0));

	else
	if(key==Key::Q)
		camera->setMove(Vec3(0,-movVel.y,0));
	else
	if(key==Key::Z)
		camera->setMove(Vec3(0,movVel.y,0));
}
void SolarFly::onMouseDown(Vec2 mousePosition, Key::Mouse button){
				
	if(button==Key::BUTTON_LEFT){
				
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