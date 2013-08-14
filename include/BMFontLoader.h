#ifndef BMFONTLOADER_H
#define BMFONTLOADER_H

#include <Config.h>
#include <Font.h>
#include <Texture.h>
#include <Utility.h>

namespace SolarSystem {

	class BMFontLoader{
	public:
		static bool load(Font& font,const Utility::Path& file);
	
	};

};

#endif