#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math2D.h>
#include <Utility.h>

namespace SolarSystem {

	class Texture {
		
		void buildTexture(void *data,uint type);

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
		Texture(const Vec4& floatColor,
				uint newWidth,
				uint newheight);
		Texture(std::vector<uchar>& bytes,
                uint width,
                uint height,
                uint format,
                uint type,
                bool mipmaps=true,
                bool bilinear=true);
		//destructor
		virtual ~Texture();
		//bind and unbind
		void bind(uint ntexture=0);
		void unbind(uint ntexture=0);
		//settings
		bool bilinear();
		bool bilinear(bool value=0);
		bool mipmaps();
		bool mipmaps(bool value=0);
		//query
		DFORCEINLINE uint getWidth()const{ return width; }    
		DFORCEINLINE uint getHeight()const{ return height; }  
		DFORCEINLINE uint getGpuID()const{ return gpuid; }    
		//overload
		bool operator ==(const Texture&) const;
		bool operator !=(const Texture&) const;
	};

	class Texture1D {
		
		void buildTexture1D(void *data,uint type);

	protected:

		bool bBilinear,chBlr;
		uint width;
        uint gpuid;
		Texture1D():bBilinear(true)
				 ,chBlr(true)
				 ,width(0)
				 ,gpuid(0){}

	public:
		//
		Texture1D(const Utility::Path& path);
		Texture1D(const Vec4& floatColor,
				  uint newWidth);
		//destructor
		virtual ~Texture1D();
		//bind and unbind
		void bind(uint ntexture=0);
		void unbind(uint ntexture=0);
		//settings
		bool bilinear();
		bool bilinear(bool value);
		//query
		DFORCEINLINE uint getWidth()const{ return width; }    
		DFORCEINLINE uint getGpuID()const{ return gpuid; }    
		//overload
		bool operator ==(const Texture1D&) const;
		bool operator !=(const Texture1D&) const;
	};

	class RenderTexture : public Texture {

		uint fboid,depthid;
		GLint defaultFBO;

	public:

		//costructor
		RenderTexture(uint width,uint height);	
		//destructor
		virtual ~RenderTexture();
		//start draw
		void enableRender();
		//end draw
		void disableRender();
		//draw in fullScreen
		void draw(bool bindTexture=true);
	};
	
	class ShadowTexture : public Texture {

		uint fboid;
		GLint defaultFBO;

	public:

		//costructor
		ShadowTexture(uint width,uint height);	
		//destructor
		virtual ~ShadowTexture();
		//start draw
		void enableRender();
		//end draw
		void disableRender();
		//draw in fullScreen
		void draw(bool bindTexture=true);
	};
};

#endif