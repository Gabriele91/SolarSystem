#ifndef SOLARMENU_H
#define SOLARMENU_H

#include <Config.h>
#include <Debug.h>
#include <Table.h>
#include <SolarButton.h>
#include <SolarRender.h>

namespace SolarSystem {

	class SolarMenu {

		std::vector< SolarButton* > buttons;
		Vec2 sizeBottons;
		Vec2 maxSizeBottons;
		Vec2 paddingBottons;
		Vec2 pixelPerSecond;

	public:
		
		SolarMenu(const Table& config);
		virtual ~SolarMenu();
		bool addOnClick(const String& name,const std::function<void()>& onClick);
		void draw(SolarRender* render);
		void update(float dt);
        void lock();
        void unlock();
		const Vec2& getPadding() const{
			return paddingBottons;
		}
		const Vec2& getSize() const{
			return sizeBottons;
		}
		const Vec2& getPixelPerSecond() const{
			return pixelPerSecond;
		}
		const Vec2& getMaxSize() const{
			return maxSizeBottons;
		}

	};

};

#endif