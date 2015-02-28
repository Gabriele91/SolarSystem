#include <stdafx.h>
#include <Camera.h>
#include <Application.h>
////////////////////////////
using namespace SolarSystem;
////////////////////////////
//set prospetive
void Camera::setPerspective(float angle,float n,float f){
	//set Perspective
	mProjMatrix.setPerspectiveRHGL(
		angle,
		Application::instance()->getScreen()->getWidth()/
		Application::instance()->getScreen()->getHeight(),
		n,
		f);
}
void Camera::setPerspective(float angle,float spectre,float n,float f){
	//set Perspective
	mProjMatrix.setPerspectiveRHGL(angle,spectre,n,f);
}
void Camera::setPerspective(float left, float right, float bottom,float top, float n, float f){
	//set Perspective
	mProjMatrix.setPerspectiveRHGL(left,right,bottom,top,n,f);
}
void Camera::setOrtogonal(float left, float right, float bottom,float top, float n, float f){
	//set Perspective
	mProjMatrix.setOrthoRHGL(left,right,bottom,top,n,f);
}

void  Camera::computePlane()
{
    const Mat4& comboMat = getViewProjMatrix();
	//compute fastrum?
    if(!mComputePlane) return;
	//left
	planes[LEFT].normal.x = comboMat[ 3] + comboMat[ 0];
	planes[LEFT].normal.y = comboMat[ 7] + comboMat[ 4];
	planes[LEFT].normal.z = comboMat[11] + comboMat[ 8];
	planes[LEFT].d =		comboMat[15] + comboMat[12];
	planes[LEFT].normalize();
	//right
	planes[RIGHT].normal.x = comboMat[ 3] - comboMat[ 0];
	planes[RIGHT].normal.y = comboMat[ 7] - comboMat[ 4];
	planes[RIGHT].normal.z = comboMat[11] - comboMat[ 8];
	planes[RIGHT].d		   = comboMat[15] - comboMat[12];
	planes[RIGHT].normalize();
	//bottom
	planes[BOTTOM].normal.x = comboMat[ 3] + comboMat[ 1];
	planes[BOTTOM].normal.y = comboMat[ 7] + comboMat[ 5];
	planes[BOTTOM].normal.z = comboMat[11] + comboMat[ 9];
	planes[BOTTOM].d        = comboMat[15] + comboMat[13];
	planes[BOTTOM].normalize();
	//top
	planes[TOP].normal.x = comboMat[ 3] - comboMat[ 1];
	planes[TOP].normal.y = comboMat[ 7] - comboMat[ 5];
	planes[TOP].normal.z = comboMat[11] - comboMat[ 9];
	planes[TOP].d        = comboMat[15] - comboMat[13];
	planes[TOP].normalize();
	//back
	planes[BACK].normal.x = comboMat[ 3] + comboMat[ 2];
	planes[BACK].normal.y = comboMat[ 7] + comboMat[ 6];
	planes[BACK].normal.z = comboMat[11] + comboMat[10];
	planes[BACK].d        = comboMat[15] + comboMat[14];
	planes[BACK].normalize();
	//front
	planes[FRONT].normal.x = comboMat[ 3] - comboMat[ 2];
	planes[FRONT].normal.y = comboMat[ 7] - comboMat[ 6];
	planes[FRONT].normal.z = comboMat[11] - comboMat[10];
	planes[FRONT].d        = comboMat[15] - comboMat[14];
	planes[FRONT].normalize();
    //is compute
    mComputePlane=false;
}
//culling methods
int Camera::sphereInFrustum(const Vec3& point, float radius){

	int result = INSIDE;
	float distance;

	for(int i=0; i < 6; ++i) {
		distance = planes[i].distance(point);
		if (distance < -radius) return OUTSIDE;
		else if (distance < radius) result = INTERSECT;
	}

	return result;

}
int Camera::pointInFrustum(const Vec3 &point){
	int result = INSIDE;

    for(int i=0; i < 6; ++i) 
		if (planes[i].distance(point) < 0)
			return OUTSIDE;

	return result;
}
//set camera
void Camera::update(){
    //compute plane
    computePlane();
	//set matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(mProjMatrix);
	//now change only MODELVIEW
	glMatrixMode(GL_MODELVIEW);
}
//get a projectate point
Vec2 Camera::getPointIn3DSpace(const Vec3& point){
	
	Vec4 vpp(point,1.0);
    vpp=getViewMatrix().mul(vpp);	
	vpp=mProjMatrix.mul(vpp);	
	if(vpp.w==0) return Vec2::ZERO;

	vpp.x/=vpp.w;
	vpp.y/=vpp.w;
	vpp.z/=vpp.w;

    /* Map x, y and z to range 0-1 */
    vpp.x = vpp.x * 0.5 + 0.5;
    vpp.y = vpp.y * 0.5 + 0.5;
    vpp.z = vpp.z * 0.5 + 0.5;    

	return vpp.xy();
}
