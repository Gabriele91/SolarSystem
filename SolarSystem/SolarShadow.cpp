#include <stdafx.h>
#include <SolarShadow.h>
#include <Debug.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

SolarShadow::SolarShadow(SolarRender* render,
						 const Vec3& point,						 
						 uint width,
						 uint height,
						 const Vec2& xyFactor)
						:render(render)
						,texture(width,height)
{
	changeDir(point);
	
	Vec2 factor=xyFactor*Vec2(texture.getWidth(),
							  texture.getHeight());
	shadowLight.setOrtogonal( -factor.x,
							   factor.x,
							  -factor.y,
							   factor.y,
							     1.0f,
							     100000.0f);
	/////////////////////////////////////////////////////////////////////////
	shadowTextureShader.loadShader("shader/shadowTexture.vs",
								   "shader/shadowTexture.ps");
	/////////////////////////////////////////////////////////////////////////
	shadowShader.shader.loadShader("shader/shadow.vs",
								   "shader/shadow.ps");
	shadowShader.glDelphMVP=shadowShader.shader.getUniformID("delphMVP");
	shadowShader.glShadowMap=shadowShader.shader.getUniformID("shadowMap");
	shadowShader.glIntesity=shadowShader.shader.getUniformID("intesity");
	/////////////////////////////////////////////////////////////////////////
}

void SolarShadow::changeDir(const Vec3& point){
	Quaternion rotation;
	rotation.setLookRotation(-point,Vec3(0,1,0));
	shadowLight.setRotation(rotation);
	shadowLight.setPosition(Vector3D::ZERO);
}

void SolarShadow::madeShadowMap(Planet *planet,const Vec3& addScale){		
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
	//disable render on color buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
	//clear z buffer
	glClear(GL_DEPTH_BUFFER_BIT);
	//Using shader texture
	shadowTextureShader.bind();	
	//get model matrix
	Mat4 model=planet->getGlobalMatrix();
	model.addScale(addScale);
	//calc MV (P in gpu)
    Mat4 viewmodel=shadowLight.getViewMatrix().mul(model);
	glLoadMatrixf(viewmodel);
	//draw planet
	planet->drawSphere();
	//disable shader texture
	shadowTextureShader.unbind();
	//enable render on color buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//disable shadow render(return to front buffer)
	texture.disableRender();
	//reset view port
    glViewport(globalViewport.x,
			   globalViewport.y,
			   globalViewport.z,
			   globalViewport.w);
	//reset old matrix
	render->setMatrixsState(globalMState);
	//get GPU errors
	CHECK_GPU_ERRORS();
	
}
void SolarShadow::drawShadow(Camera *camera,Planet *planet,float intesity){
	//z-buffer fixed
	float distToCam=camera->getPosition(true).distance(-planet->getPosition(true));
	float scaleShadow=distToCam<planet->getScale().length()*10 ? 1.01f : 1.1f;
	//get model matrix
	Mat4 model=planet->getGlobalMatrix();
	model.addScale(Vec3(scaleShadow,scaleShadow,scaleShadow));
	//bind shader
	shadowShader.shader.bind();
	//calc delphMVP   (Proj*View)*model
	Mat4 delphMVP=shadowLight.getViewProjMatrix().mul(model);
	shadowShader.shader.uniformMatrix4x4(shadowShader.glDelphMVP,  delphMVP );
	//calc MV (P in gpu)
	Mat4 viewmodel=camera->getViewMatrix().mul(model);
	glLoadMatrixf(viewmodel);
	//bind texture
	texture.bind(0);
	shadowShader.shader.uniformInt(shadowShader.glShadowMap,0);
	shadowShader.shader.uniformFloat(shadowShader.glIntesity,intesity);
	//draw planet
	planet->drawSphere();
	//unbind texture
	texture.unbind(0);
	//unbind shader
	shadowShader.shader.unbind();
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