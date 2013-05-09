#include <stdafx.h>
#include <SolarShadow.h>
#include <Debug.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

SolarShadow::SolarShadow(SolarRender* render,
						 const Vec3& point)
						:render(render)
						,texture(512,512)
{
	changeDir(point);
	float factor=0.5;
	float factorX=factor*texture.getWidth();
	float factorY=factor*texture.getHeight();
	shadowLight.setOrtogonal( -factorX,
							   factorX,
							  -factorY,
							   factorY,
							   1.0f,
							   100000.0f);
}

void SolarShadow::changeDir(const Vec3& point){
	Quaternion rotation;
	rotation.setLookRotation(-point,Vec3(0,1,0));
	shadowLight.setRotation(rotation);
	shadowLight.setPosition(Vector3D::ZERO);
}

void SolarShadow::enableRender(){		
	//save viewport
	glGetFloatv(GL_VIEWPORT,&globalViewport.x);
	//save matrixs
	render->getMatrixsState(globalMState);
	//update local mat4s
	shadowLight.update();
	//enable shadow render
	texture.enableRender();
	//set viewport
    glViewport(0, 0,  texture.getWidth(), texture.getHeight());
#ifdef SHADOW_SHOW_CAMERA
	render->setClearColor(Vec4(0.2,0.5,1.0,1.0));
#else
	//disable render on color buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
	//clear z buffer
	glClear(GL_COLOR_BUFFER_BIT);
#endif
	//disable textures
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	//Using the fixed pipeline
	glUseProgram(0);

}
void SolarShadow::disableRender(){	
	//disable shadow render(return to front buffer)
	texture.disableRender();
	//reset old matrix
	render->setMatrixsState(globalMState);
	//reset view port
    glViewport(globalViewport.x,
			   globalViewport.y,
			   globalViewport.z,
			   globalViewport.w);

#if !defined(SHADOW_SHOW_CAMERA)
	//enable render on color buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
#endif
	//renable textures
	glEnable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_2D);
	//get GPU errors
	CHECK_GPU_ERRORS();

}
void SolarShadow::draw(){
	if(render->zBufferIsEnable()){
		render->disableZBuffer();
		texture.draw();
		render->enableZBuffer();
	}
	else
		texture.draw();
}