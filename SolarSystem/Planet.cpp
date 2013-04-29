#include <stdafx.h>
#include <Planet.h>
#include <Debug.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

void Planet::buildMesh(int rings,int sectors){
	// make planet mesh 
	verticesSize=rings*sectors;
	indicesSize=rings*sectors*6;
	GLVertex	 *vertices=new GLVertex[verticesSize];
	GLushort	 *indices=new GLushort[indicesSize];
	//set vertices
	float const R = 1.0f/(float)(rings-1);
	float const S = 1.0f/(float)(sectors-1);
	int idv=0;
	for(int r = 0; r<rings; ++r){
		for(int s= 0; s<sectors; ++s){					
			float const y = sin( -0.5*Math::PI + Math::PI * r * R );
			float const x = cos(  2*Math::PI * s * S) * sin( Math::PI * r * R );
			float const z = sin(  2*Math::PI * s * S) * sin( Math::PI * r * R );
			/* calc vertex */
			vertices[idv].vertices.x= x * 1;
			vertices[idv].vertices.y= y * 1;
			vertices[idv].vertices.z= z * 1;
			/* calc normal */
			vertices[idv].normals.x= x;
			vertices[idv].normals.y= y;
			vertices[idv].normals.z= z;
			/* calc uv */
			vertices[idv].texcoords.x=s*S;
			vertices[idv].texcoords.y=r*R;
			//
			++idv;					
		}
	}
	//set indices
	int i = -1;
    for(int r = 0; r < rings-1; r++) {
		for(int s = 0; s < sectors-1; s++) {
			//1 2 3
            indices[++i] = r * sectors + s;
            indices[++i] = r * sectors + (s+1);
            indices[++i] = (r+1) * sectors + (s+1);
			//1 3 4
            indices[++i] = r * sectors + s;
            indices[++i] = (r+1) * sectors + (s+1);
            indices[++i] = (r+1) * sectors + s;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//send to GPU 			
	if( !verticesBuffer )
		glGenBuffers(1, &verticesBuffer );
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex) * verticesSize, &vertices[0], GL_STATIC_DRAW);
	if( !indicesBuffer )
		glGenBuffers(1, &indicesBuffer );		
	glBindBuffer(GL_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ushort) * indicesSize, &indices[0], GL_STATIC_DRAW);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//free cpu memory
	delete[] vertices;
	delete[] indices;
}
void Planet::bindMesh(){
	//bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, verticesBuffer );
	//set vertex
	glVertexPointer  (3, GL_FLOAT, sizeof(GLVertex), 0 );
	glNormalPointer  (   GL_FLOAT, sizeof(GLVertex), (void*)offsetof(GLVertex,normals) );
	glTexCoordPointer(2, GL_FLOAT, sizeof(GLVertex), (void*)offsetof(GLVertex,texcoords) );
	//bind IBO
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indicesBuffer ); 
	//draw
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_SHORT, 0);
}
Planet::Planet(SolarRender *render,
			   const Utility::Path& texture,
			   int rings,
			   int sectors)
			  :verticesSize(0)
			  ,verticesBuffer(0)
			  ,indicesSize(0)
			  ,indicesBuffer(0)
			  ,texture(texture)
			  ,cloudTexture(NULL)
			  ,blackTexture(NULL)
			  ,render(render)
			  ,ambient(Vec3::ZERO,1.0f)
			  ,diffuse(Vec3::ZERO,1.0f)
			  ,specular(Vec3::ZERO,1.0f)
			  ,emission(Vec3::ZERO,1.0f)
			  ,shininess(1.0){
	//build mesh
	buildMesh(rings,sectors);
}
Planet::~Planet(){
	if(cloudTexture) 
		delete cloudTexture;
	if(blackTexture) 
		delete blackTexture;
}
//set extra texture
void Planet::setCloudTexture(const Utility::Path& texture){
	DEBUG_ASSERT(cloudTexture==NULL);
	cloudTexture=new Texture(texture);
}
void Planet::setBlackTexture(const Utility::Path& texture){
	DEBUG_ASSERT(blackTexture==NULL);
	blackTexture=new Texture(texture);
};
//draw
void Planet::draw(Camera& camera){
	////////////////////////////////////
	//get values
	const Mat4& thisMtx=getGlobalMatrix();
	const Vec3& thisScale=thisMtx.getScale3D();
	const float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z));
	//culling
	if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE) {
		if(render->lightIsEnable()){	
			render->setMaterial(ambient,
								diffuse,
								specular,
								emission,
								shininess);
			
			if(blackTexture) blackTexture->bind(1);
			else render->getTextureBlack().bind(1);
		}
		//bind texture 
		texture.bind(0);
		//set model matrix
		Mat4 viewmodel=camera.getGlobalMatrix().mul(thisMtx);
		glLoadMatrixf(viewmodel);
		//draw
		bindMesh();
		//unbind
		if(render->lightIsEnable()){			
			if(blackTexture) blackTexture->unbind(1);
			else render->getTextureBlack().unbind(1);
		}
		//draw cloud
		if(cloudTexture && 
		   camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale*1.01) != Camera::OUTSIDE){			
			//save blend
			auto stateBlend=render->getBlendState();
			//set additive blend
			if(!stateBlend.enable) glEnable( GL_BLEND );   
			glBlendFunc( GL_SRC_ALPHA, GL_ONE  );
			//
			Vec3 cloudScale(1.01f,1.01f,1.01f);
			//set matrix
			viewmodel.entries[0]*=cloudScale.x; viewmodel.entries[1]*=cloudScale.y; viewmodel.entries[2]*=cloudScale.z;
			viewmodel.entries[4]*=cloudScale.x; viewmodel.entries[5]*=cloudScale.y; viewmodel.entries[6]*=cloudScale.z;
			viewmodel.entries[8]*=cloudScale.x; viewmodel.entries[9]*=cloudScale.y;	viewmodel.entries[10]*=cloudScale.z;			
			//
			glLoadMatrixf(viewmodel);
			//bind texture
			cloudTexture->bind(0);
			//set black texture
			if(render->lightIsEnable()) render->getTextureBlack().bind(1);
			//draw
			bindMesh();
			//unbind black texture		
			if(render->lightIsEnable()) render->getTextureBlack().unbind(1);
			//reset old blend state  
			render->setBlendState(stateBlend);
		}
	}

}
//set data
void Planet::setData(float _day){
	//save
	day=_day;
	//calc angle
	float angle=(day/daysOfYear)*Math::PI2;
	//calc Position
	Vec3 pos;	
	pos.x = cos(angle) * radius.x;
    pos.z = sin(angle) * radius.y;
	//////////////////////////////
	//calc offset sun
	float value=(radius.x-radius.y)/2.0;
	//////////////////////////////
	if(value<0.0f)
		pos.z+=value;
	else
	if(value>0.0f)
		pos.x-=value;
	//set Position
	setPosition(pos);
	//calc rotation
	float yRotation=day/rotationPeriod*Math::PI2;
	Quaternion rot;
	rot.setFromEulero(0,yRotation,0);
	setRotation(rot);
}
//set planet info
void Planet::setPlanetInfo(const Vec2& _radius, 
						   float _daysOfYear,
						   float _rotationPeriod){
	radius=_radius;
	daysOfYear=_daysOfYear;
	rotationPeriod=_rotationPeriod;
}
//set material		
void Planet::setMaterial(const Vec4& _ambient,
						const Vec4& _diffuse,
						const Vec4& _specular,
						const Vec4& _emission,
						float _shininess){
	ambient=_ambient;
	diffuse=_diffuse;
	specular=_specular;
	emission=_emission;
	shininess=_shininess;
}