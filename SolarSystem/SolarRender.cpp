#include <stdafx.h>
#include <SolarRender.h>
#include <Application.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

SolarRender::SolarRender(){}

void SolarRender::init(){
	//OPENGL
	//view port
	glViewport(0, 0, Application::instance()->getScreen()->getWidth(),
						Application::instance()->getScreen()->getHeight());
	//enable culling
	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	//enable z buffer
	glEnable(GL_DEPTH_TEST);
	//enable texturing	
	glEnable( GL_TEXTURE_2D );
	//enable state	
	//always active!
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);       
	//default status for blending    
	glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );   
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}
void SolarRender::setCullFace(SolarRender::CULLFACE type){
	glEnable( GL_CULL_FACE );
	glCullFace( type );
}
void SolarRender::setClearColor(const Vec4& color){	
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}