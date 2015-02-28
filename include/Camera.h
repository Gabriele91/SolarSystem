#ifndef CAMERA_H
#define CAMERA_H

#include <Config.h>
#include <Object.h>


namespace SolarSystem {

	class Camera : public Object {

		enum{
			BACK=0,
			FRONT=1,
			TOP=2,
			BOTTOM=3,
			LEFT=4,
			RIGHT=5
		};
		//planes
		Plane planes[6];
        bool  mComputePlane;
        void  computePlane();
		//matrix
		Mat4 mProjMatrix;
		Mat4 mViewMatrix;
		Mat4 mViewProjMatrix;
		Mat4 mModelProjMatrix;
        //update matrixs
        void updateMatrix()
        {
            if(isChange())
            {
                const Mat4& model= Object::getGlobalMatrix();
                mViewMatrix      = model.getInverse();
                mViewProjMatrix  = mProjMatrix.mul(mViewMatrix);
                mModelProjMatrix = mProjMatrix.mul(model);
                mComputePlane    = true;
            }
        }
		//
	public:
		//
		Camera():Object(),mComputePlane(true){}
		//set prospetive
		void setPerspective(float angle,float n,float f);
		void setPerspective(float angle,float spectre,float n,float f);
		void setPerspective(float left, float right, float bottom,float top, float n, float f);
		void setOrtogonal(float left, float right, float bottom,float top, float n, float f);
		//culling methods
		enum {OUTSIDE, INTERSECT, INSIDE};
		int sphereInFrustum(const Vec3& point, float radius);
		int pointInFrustum(const Vec3 &point);
		//update camera
		void update();
		//return matrix
		const Mat4& getProjectionMatrix(){
			return mProjMatrix;
		}
		const Mat4& getViewProjMatrix(){
            updateMatrix();
			return mViewProjMatrix;
		}
		const Mat4& getViewMatrix(){
            updateMatrix();
            return mViewMatrix;
		}
		const Mat4& getGlobalMatrix(){
            updateMatrix();
            return Object::getGlobalMatrix();
		}
		const Mat4& getModelProjMatrix(){
            updateMatrix();
			return mModelProjMatrix;
		}
		Vec2 getPointIn3DSpace(const Vec3& point);
	};

};

#endif
