#include <stdafx.h>
#include <Texture.h>
#include <Application.h>
#include <Debug.h>
#define IMAGE_LOADER_OPENGL
#include "Image/Image.h"

///////////////////////
using namespace SolarSystem;
///////////////////////
Texture::Texture(const Utility::Path& path)
				:bBilinear(true)
				,chBlr(true)
				,bMipmaps(true)
				,chMps(true)
				,width(0)
				,height(0)
				,gpuid(0){
	/////////////////////////////////////////////////////////////////////
	//cpu load
	//get raw file
	void *data=NULL; uint len=0;
	Application::instance()->loadData(path,data,len);
	//load image
	Image image;
	image.loadFromData(data,
					   len,
					   Image::getTypeFromExtetion(path.getExtension()));
	//free raw file
	free(data);
	/////////////////////////////////////////////////////////////////////
	//gen gpu
	//create an GPU texture
	glGenTextures( 1, &gpuid );
	//build
	bind();
	//save width end height
	width=image.width;
	height=image.height;	
	//create a gpu texture
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 image.type,
				 width,
				 height,
				 0,
				 image.type,
				 GL_UNSIGNED_BYTE,
				 0);
	//create mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, bMipmaps );	
	//send to GPU
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0,
					 width,
					 height,
					 image.type,
					 GL_UNSIGNED_BYTE,
					 image.bytes );
	//////////////////////////////////
	//get errors...
	CHECK_GPU_ERRORS();
}
//destructor
Texture::~Texture(){
	//unload
	DEBUG_ASSERT(gpuid);
    glDeleteTextures(1, &gpuid );
	//reset values
    width = height = 0;
    gpuid = 0;
}
//
void Texture::bind(uint ntexture){
	//
	DEBUG_ASSERT(gpuid);
	//
    glActiveTexture( GL_TEXTURE0 + ntexture );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, (GLuint)gpuid );
	//settings
	if(chBlr)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,bBilinear?GL_LINEAR:GL_NEAREST);
	if(chMps)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,bMipmaps?GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR);
}
//setting
bool Texture::bilinear(){
	return bBilinear;
}
bool Texture::bilinear(bool value){
	chBlr=bBilinear!=value;
	return bBilinear=value;
}
bool Texture::mipmaps(){
	return bMipmaps;
}
bool Texture::mipmaps(bool value){
	chMps=bMipmaps!=value;
	return bMipmaps=value;
}
//operators
bool Texture::operator ==(const Texture& t) const{
	return gpuid==t.gpuid;
}
bool Texture::operator !=(const Texture& t) const{
	return gpuid!=t.gpuid;
}
