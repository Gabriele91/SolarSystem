#ifndef SOLARBUTTON_H
#define SOLARBUTTON_H

#include <functional>
#include <Config.h>
#include <Texture.h>
#include <Table.h>
#include <EString.h>
#include <Object.h>
#include <Input.h>

namespace SolarSystem {
	//dad
	class SolarMenu;
	//childs
	class SolarButton : public Object,
							   Input::MouseHandler{
		
		enum StateBotton{
			NORMAL=0,
			OVERED,
			ACTIVE
		};

		StateBotton state;
		Texture *textures[3];
		std::function<void()> onClick;
		String name;
        bool islocked;
		int id;

		//mouse in box
		bool mouseInBox(const Vec2& mouse);

		//mouse
		virtual void onMouseMove(Vec2 mousePosition) ;
		virtual void onMousePress(Vec2 mousePosition, Key::Mouse button);
		virtual void onMouseDown(Vec2 mousePosition, Key::Mouse button);
		virtual void onMouseRelease(Vec2 mousePosition, Key::Mouse button);
		virtual void onMouseScroll(short scrollDelta);
        

	public:

		SolarButton(const String& name,const Table& config);
		virtual ~SolarButton();
		bool isCalled(const String& name);
		void addOnClick(const std::function<void()>& onClick){
			this->onClick=onClick;
		}
		Texture* getCurrentTexture(){
			return textures[state];
		}
		int getID(){
			return id;
		}
        
        void lock();
        void unlock();
		void update(SolarMenu *menu,float dt);

	};

};

#endif