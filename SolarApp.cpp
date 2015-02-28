#include <stdafx.h>
#include <Application.h>
#include <SolarApp.h>

using namespace SolarSystem;


void SolarApp::drawLoading(){

	//save state
	Vec4 globalViewport;
	SolarRender::BlendState blendState;
	SolarRender::MatrixsState matrixsState;
	blendState=render.getBlendState();
	render.getMatrixsState(matrixsState);	
	glGetFloatv(GL_VIEWPORT,&globalViewport.x);
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
	projection.setOrthoRHGL(0,windowSize.x,0,windowSize.y,0.0,1.0);
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
    glDisableClientState(GL_NORMAL_ARRAY);

	//matrix button
	glLoadMatrixf(loadingScreenMatrix);
	//texture
	loadingScreen->bind();
	//set vertex
	glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
	//draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//restore state
    glEnableClientState(GL_NORMAL_ARRAY);
	render.enableZBuffer();
	render.setBlendState(blendState);
	render.setMatrixsState(matrixsState);
    glViewport(globalViewport.x,
			   globalViewport.y,
			   globalViewport.z,
			   globalViewport.w);

}
