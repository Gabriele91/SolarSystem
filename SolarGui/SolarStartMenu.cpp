#include <stdafx.h>
#include <SolarStartMenu.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

void SolarStartMenu::drawTexture(Texture* texture,const Matrix4x4& tranform){
	render.disableZBuffer();
	//query
	Screen *screen=Application::instance()->getScreen();
	Vec2 windowSize(screen->getWidth(),screen->getHeight());
	//draw objects
	//disable shader
    glUseProgram(0);
	//ortogonal mode (2d)	
    glViewport(0,0,windowSize.x,windowSize.y);
	Matrix4x4 projection;
	projection.setOrtho(0,windowSize.x,0,windowSize.y,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);
	glMatrixMode(GL_MODELVIEW);
	//draw
	static const float
	xyUV[]={
			-0.5,-0.5,0.0,0.0,
			-0.5, 0.5,0.0,1.0,
			 0.5,-0.5,1.0,0.0,
			 0.5, 0.5,1.0,1.0
	        };
	//disable vbo
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	//matrix logo
	glLoadMatrixf(tranform);
	//texture
	texture->bind();
	//set vertex
	glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
	//draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//restore state
	render.enableZBuffer();
}