#include <stdafx.h>
#include <SolarRings.h>
#include <Debug.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
void SolarRings::buildMesh(float nr,float fr,int sectors){

	verticesSize=sectors*2;
	indicesSize=sectors*2+2;
	GLVertex	 *vertices=new GLVertex[verticesSize];
	GLushort	 *indices=new GLushort[indicesSize];


	
	float S=Math::PI/(float)sectors;
	for(int s=0;s<sectors*2;s+=2){
		
		float _sin = std::sin( s*S );
		float _cos = std::cos( s*S );
		
		vertices[s].vertices.x=_cos*nr;
		vertices[s].vertices.y=0.0;
		vertices[s].vertices.z=_sin*nr;		
		vertices[s].normals=vertices[s].vertices.getNormalize();
		vertices[s].texcoords.x=0.0;
		vertices[s].texcoords.y=0.0;

		vertices[s+1].vertices.x=_cos*fr;
		vertices[s+1].vertices.y=0.0;
		vertices[s+1].vertices.z=_sin*fr;		
		vertices[s+1].normals=vertices[s+1].vertices.getNormalize();
		vertices[s+1].texcoords.x=1.0;
		vertices[s+1].texcoords.y=0.0;


		
	}

	int countV=0;
	int i=0;
	for(;i<indicesSize-2;++i){	
		indices[i]=i;
	}
	indices[i++]=0;
	indices[i]=1;

	//bind gpu buffers
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
	//get errors...
	CHECK_GPU_ERRORS();
}
void SolarRings::bindMesh(){	
	//get status
	bool statusOld=render->cullFaceIsEnable();
	//disable cullface
	if(statusOld) render->disableCullFace();
	//bind VBO
	glBindBuffer( GL_ARRAY_BUFFER, verticesBuffer );
	//set vertex
	glVertexPointer  (3, GL_FLOAT, sizeof(GLVertex), 0 );
	glNormalPointer  (   GL_FLOAT, sizeof(GLVertex), (void*)offsetof(GLVertex,normals) );
	glTexCoordPointer(2, GL_FLOAT, sizeof(GLVertex), (void*)offsetof(GLVertex,texcoords) );
	//bind IBO
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indicesBuffer ); 
	//draw
	glDrawElements(GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_SHORT, 0);
	//enable cullface
	if(statusOld) render->enableCullFace();
}

SolarRings::SolarRings(SolarRender *render,
						const Utility::Path& texture,
						float nr,
						float fr,
						int sectors)
					   :render(render)
					   ,diffuseTexture(texture)
					   ,frSize(fr)
					   ,nrSize(nr)
					   ,verticesBuffer(0)
					   ,indicesBuffer(0)
					   ,verticesSize(0)
					   ,indicesSize(0)
{
	buildMesh(nr,fr,sectors);
}
void SolarRings::drawRing(){
	bindMesh();
}
void SolarRings::drawBase(Camera& camera){
		//set model matrix
		Mat4 viewmodel=camera
			           .getGlobalMatrix()
					   .mul(getGlobalMatrix());
		glLoadMatrixf(viewmodel);
		//draw
		bindMesh();
}
void SolarRings::draw(Camera& camera){
	
	const Mat4& thisMtx=getGlobalMatrix();
	const Vec3& thisScale=thisMtx.getScale3D();
	const float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z))*frSize;
	//in screen
	if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale)){
		//set texture
		diffuseTexture.bind(0);
		//set model matrix
        Mat4 viewmodel=camera.getViewMatrix().mul(thisMtx);
		glLoadMatrixf(viewmodel);
		//draw
		bindMesh();
		//disable texture
		diffuseTexture.unbind(0);
		//
	}
}
