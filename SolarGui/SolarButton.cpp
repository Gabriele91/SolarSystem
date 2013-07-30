#include <stdafx.h>
#include <SolarButton.h>
#include <SolarMenu.h>
#include <Application.h>
#include <Debug.h>

///////////////////////
using namespace SolarSystem;
///////////////////////


SolarButton::SolarButton(const String& name,const Table& config):state(NORMAL),name(name),onClick([](){}){
	//chack textures parameters exists
	if(!config.existsAsType("normal",Table::STRING)||
	   !config.existsAsType("overed",Table::STRING)||
	   !config.existsAsType("active",Table::STRING)	){		
			DEBUG_ASSERT_MSG(0,"SolarButton error : "+
								name+
								" must to be setted normal, overed, active, params");	
	}
	//load textures
	textures[NORMAL]=new Texture(config.getTablePath().getDirectory()+"/"+config.getString("normal"));
	textures[OVERED]=new Texture(config.getTablePath().getDirectory()+"/"+config.getString("overed"));
	textures[ACTIVE]=new Texture(config.getTablePath().getDirectory()+"/"+config.getString("active"));
	//add handler input
	Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);

}
SolarButton::~SolarButton(){
		for(auto texture:textures)
			delete texture;	
		//delete handler input
		Application::instance()->getInput()->removeHandler((Input::MouseHandler*)this);
}

bool SolarButton::mouseInBox(const Vec2& mouse){
	Vec2 vmouse(mouse.x,Application::instance()->getScreen()->getHeight()-mouse.y);
	Vec2 a(getPosition().xy()-getScale().xy()*0.5);
	Vec2 b(getPosition().xy()+getScale().xy()*0.5);
	bool bx= a.x<=vmouse.x && vmouse.x<=b.x;
	bool by= a.y<=vmouse.y && vmouse.y<=b.y;
	return bx && by;
}
void SolarButton::onMouseMove(Vec2 mousePosition) {
	if(mouseInBox(mousePosition)) {
		if(state==NORMAL) 
			state=OVERED;
	}
	else 
		state=NORMAL;
}
void SolarButton::onMousePress(Vec2 mousePosition, Key::Mouse button) {
	if(state==OVERED) state=ACTIVE;
}
void SolarButton::onMouseDown(Vec2 mousePosition, Key::Mouse button) {
	if(state==OVERED) state=ACTIVE;
}
void SolarButton::onMouseRelease(Vec2 mousePosition, Key::Mouse button) {
	if(state==ACTIVE){
		onClick();
		state=OVERED;
	}
}
void SolarButton::onMouseScroll(short scrollDelta) {}

void SolarButton::update(SolarMenu *menu,float dt){
	Vec2 animVel=menu->getPixelPerSecond();
	if(state==NORMAL){
		//animation
		if(getScale().x>menu->getSize().x)
			setScale(Vec3(getScale().x-animVel.x*dt,getScale().y,1.0));
		if(getScale().y>menu->getSize().y)
			setScale(Vec3(getScale().x,getScale().y-animVel.y*dt,1.0));
		//limit
		if(getScale().x<menu->getSize().x)
			setScale(Vec3(menu->getSize().x,getScale().y,1.0));
		if(getScale().y<menu->getSize().y)
			setScale(Vec3(getScale().x,menu->getSize().y,1.0));

	}
	else if(state==OVERED||state==ACTIVE){
		//animation
		if(getScale().x<menu->getMaxSize().x)
			setScale(Vec3(getScale().x+animVel.x*dt,getScale().y,1.0));
		if(getScale().y<menu->getMaxSize().y)
			setScale(Vec3(getScale().x,getScale().y+animVel.y*dt,1.0));	
		//limit
		if(getScale().y>menu->getMaxSize().x)
			setScale(Vec3(menu->getMaxSize().x,getScale().y,1.0));	
		if(getScale().y>menu->getMaxSize().y)
			setScale(Vec3(getScale().x,menu->getMaxSize().y,1.0));	

	}
}

bool SolarButton::isCalled(const String& name){
		return this->name==name;
}