#ifndef TEXTURE_H
#define TEXTURE_H

#include <Config.h>
#include <Math2D.h>
#include <Utility.h>

namespace SolarSystem {



	class RenderTexture : public Texture {

		uint fboid,depthid;

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

};

#endif