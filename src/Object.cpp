#include <stdafx.h>
#include <Object.h>
/* Easy3D */
using namespace SolarSystem;
//////////////////////
Object::Object()
			   :data(NULL),
		        parentMode(DISABLE_PARENT),
		        parent(NULL),
				changeValue(false){}
Object::~Object(){
	for(auto obj : *this){
		obj->parent=NULL;
		delete obj;
	}
	childs.clear();
}

void Object::setScale(const Vector3D &scale,bool global){
	if(!global||!parent)
		transform.scale=scale;
	else{
		//transform.scale=scale*(transform.scale/getGlobalMatrix().getScale3D());
		transform.scale/=getGlobalMatrix().getScale3D();
		transform.scale*=scale;
	}
	change();
}
void Object::setPosition(const Vector3D &position,bool global){
	if(!global||!parent)
		transform.position=position;
	else
		//transform.position=position+(transform.position-getGlobalMatrix().getTranslation3D());
		transform.position-=getGlobalMatrix().getTranslation3D()-position;

	change();
}
void Object::setRotation(const Quaternion& rotation,bool global){
	if(!global||!parent)
		transform.rotation=rotation;	
	else{
		const Matrix4x4& gm4x4=getGlobalMatrix();
		Quaternion tmpRot;
		tmpRot.setFromEulero(gm4x4.getRotX(),
							 gm4x4.getRotY(),
							 gm4x4.getRotZ());
		transform.rotation=rotation-tmpRot;
	}
	change();
}

void Object::setTranslation(const Vector3D &translation){
	transform.position+=translation;
	change();
}
void Object::setTurn(const Quaternion& rotation){

	Vec3 local,turn;
	transform.rotation.getEulero(local.x,local.y,local.z);
	rotation.getEulero(turn.x,turn.y,turn.z);
	transform.rotation.setFromEulero(local.x+turn.x,
									 local.y+turn.y,
									 local.z+turn.z);
	change();
}
//
Vector3D Object::getScale(bool global){
	if(!global||!parent)
		return transform.scale;
	else
		return getGlobalMatrix().getScale3D();
}
Vector3D Object::getPosition(bool global){
	if(!global||!parent)
		return transform.position;
	else
		return getGlobalMatrix().getTranslation3D();
}
Quaternion Object::getRotation(bool global){
	if(!global||!parent)
		return transform.rotation;
	else{
		//TO DO: better method....
		Quaternion out;
		const Matrix4x4& global=getGlobalMatrix();
		out.setFromEulero(global.getRotX(),
						  global.getRotY(),
						  global.getRotZ());
		return out;
	}
}
Object::ParentMode Object::getParentMode() const{
	return parentMode;
}
//
void Object::change(){
	if(!changeValue){
		for(auto obj : *this ){
		  obj->changeValue=true;
		  obj->change();
		}
		changeValue=true;
	}
}
void Object::addChild(Object *child,ParentMode type){

	if(child->parent==this) return;
	if(child==this) return;
	if(child->parent) child->parent->erseChild(child);

	child->parent=this;
	child->parentMode=type;
	this->childs.push_back(child);

}
void Object::erseChild(Object *child){
	if(child->parent==this){
		childs.remove(child);
		child->parent=NULL;
	}
}
//
std::list<Object*>::iterator Object::begin(){ return childs.begin(); }
std::list<Object*>::iterator Object::end(){ return childs.end(); }
std::list<Object*>::reverse_iterator Object::rbegin(){ return childs.rbegin(); }
std::list<Object*>::reverse_iterator Object::rend(){ return childs.rend(); }
//
const Matrix4x4& Object::getGlobalMatrix(){
	if(changeValue==true){
		//
		globalMat.identity();
		//
		if(parent){
			Matrix4x4 mtmp,ptmp;
			//calc values
			mtmp=parent->getGlobalMatrix();
			Vector3D tmpPos(mtmp.entries[12],mtmp.entries[13],mtmp.entries[14]);
			Quaternion tmpRot;
			tmpRot.setFromEulero(mtmp.getRotX(),
								 mtmp.getRotY(),
								 mtmp.getRotZ());
			mtmp.identity();
			//
			if(parentMode & (ENABLE_PARENT)){
				//////////////////////////////////////////////
				//rotarion local
				globalMat=transform.rotation.getMatrix();				
				//position local
				ptmp.entries[12]=transform.position.x;
				ptmp.entries[13]=transform.position.y;
				ptmp.entries[14]=transform.position.z;
				globalMat=globalMat.mul(ptmp);
				//////////////////////////////////////////////
				//rotarion global
				mtmp=tmpRot.getMatrix();
				//position global
				ptmp.entries[12]=tmpPos.x;
				ptmp.entries[13]=tmpPos.y;
				ptmp.entries[14]=tmpPos.z;	
				mtmp=mtmp.mul(ptmp);		
				//global*local
				globalMat=mtmp.mul(globalMat);
				//////////////////////////////////////////////
			}

			if(parentMode & ENABLE_SCALE){
			   const Vector3D& vTScale=getGlobalParentScale();
			   mtmp.setScale(vTScale);
			   globalMat=globalMat.mul(mtmp);
			}
			else{
			   mtmp.setScale(transform.scale);
			   globalMat=globalMat.mul(mtmp);
			}

		}
		else{
			//rotarion
			globalMat=transform.rotation.getMatrix();
			//position
			Mat4 tmp;
			tmp.entries[12]=transform.position.x;
			tmp.entries[13]=transform.position.y;
			tmp.entries[14]=transform.position.z;
			globalMat=globalMat.mul(tmp);
			//scale
			globalMat.entries[0]*=transform.scale.x;
			globalMat.entries[5]*=transform.scale.y;
			globalMat.entries[10]*=transform.scale.z;
		}
		//
		changeValue=false;
	 }

return globalMat;
}
Vector3D  Object::getGlobalParentScale(){
           //no steck over flu....
           Object *p=NULL;
           Vector3D out=transform.scale;
           for(p=this->parent; p; p=p->parent ){
                out*=p->transform.scale;
           }
           return out;
}

