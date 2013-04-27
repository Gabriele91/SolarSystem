#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Utility.h>

namespace SolarSystem {

	class Texture {

	protected:

		bool bBilinear,chBlr,bMipmaps,chMps;
		uint width,height;
        uint gpuid;
		Texture():bBilinear(true)
				 ,chBlr(true)
				 ,bMipmaps(true)
				 ,chMps(true)
				 ,width(0)
				 ,height(0)
				 ,gpuid(0){}

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
		DFORCEINLINE uint getWidth()const{ return width; }    
		DFORCEINLINE uint getHeight()const{ return height; }  
		DFORCEINLINE uint getGpuID()const{ return gpuid; }    
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
	};

	class RenderTexture : public Texture {

		uint fboid;

	public:

		//costructor
		RenderTexture(uint width,uint height);	
		//destructor
		virtual ~RenderTexture();
		//start draw
		void enableRender();
		//end draw
		void disableRender();

	};

};

#endif