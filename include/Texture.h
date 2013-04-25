#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Utility.h>

namespace SolarSystem {

	class Texture {

		bool bBilinear,chBlr,bMipmaps,chMps;
		uint width,height;
        uint gpuid;

	public:
		//
		Texture(const Utility::Path& path);	
		//destructor
		virtual ~Texture();
		//
		void bind(uint ntexture=0);
		//settings
		bool bilinear();
		bool bilinear(bool value);
		bool mipmaps();
		bool mipmaps(bool value);
		//query
		DFORCEINLINE uint getWidth(){ return width; }    
		DFORCEINLINE uint getHeight(){ return height; }    
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
	};

};

#endif