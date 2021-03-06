#include <stdafx.h>
#include <SolarSky.h>
#include <Debug.h>
///////////////////////
using namespace SolarSystem;
///////////////////////

void SolarSky::buildSkyBox(){
	static const float
	size=10.0;
	static const float
	offset=0.00;	
	static const float
	offsetUV1=0.9995;	
	static const float
	offsetUV0=0.0005;	
	//structs
	/////////////////////////////////////
	static const float
	front[]={ 
			-size,-size,-size+offset,offsetUV0,offsetUV0, 
			-size, size,-size+offset,offsetUV0,offsetUV1,  
			 size,-size,-size+offset,offsetUV1,offsetUV0, 
			 size, size,-size+offset,offsetUV1,offsetUV1
	        };
	/////////////////////////////////////
	static const float
	left[]={ 
			 -size+offset, size,-size,offsetUV1,offsetUV1, 
			 -size+offset,-size,-size,offsetUV1,offsetUV0,
			 -size+offset, size, size,offsetUV0,offsetUV1,  
			 -size+offset,-size, size,offsetUV0,0.0
	        };
	/////////////////////////////////////
	static const float
	back[]={  
			-size, size,size-offset,offsetUV1,offsetUV1,
			-size,-size,size-offset,offsetUV1,offsetUV0,
			 size, size,size-offset,offsetUV0,offsetUV1,  
			 size,-size,size-offset,offsetUV0,offsetUV0 
	        };
	/////////////////////////////////////
	static const float
	right[]={ 
			 size-offset,-size,-size,offsetUV0,offsetUV0, 
			 size-offset, size,-size,offsetUV0,offsetUV1,  
			 size-offset,-size, size,offsetUV1,offsetUV0, 
			 size-offset, size, size,offsetUV1,offsetUV1
	        };
	/////////////////////////////////////
	static const float
	top[]={ 
			-size, size-offset,-size,offsetUV0,offsetUV0, 
			-size, size-offset, size,offsetUV0,offsetUV1,  
			 size, size-offset,-size,offsetUV1,offsetUV0, 
			 size, size-offset, size,offsetUV1,offsetUV1
	        };
	/////////////////////////////////////
	static const float
	bottom[]={ 
			-size, -size+offset, size,offsetUV0,offsetUV0, 
			-size, -size+offset,-size,offsetUV0,offsetUV1, 
			 size, -size+offset, size,offsetUV1,offsetUV0,
			 size, -size+offset,-size,offsetUV1,offsetUV1,
	        };
/////////////////////////////////////	
#define BUILD_GPU_BUFFER(gpubfr,cpubuffer)\
	if( !(gpubfr) )\
		glGenBuffers(1, &(gpubfr) );\
	glBindBuffer(GL_ARRAY_BUFFER,  (gpubfr) );\
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*5*4, & cpubuffer [0], GL_STATIC_DRAW);\
	CHECK_GPU_ERRORS();


	BUILD_GPU_BUFFER(buffer.front,front)
	BUILD_GPU_BUFFER(buffer.back,back)
	BUILD_GPU_BUFFER(buffer.top,top)
	BUILD_GPU_BUFFER(buffer.bottom,bottom)
	BUILD_GPU_BUFFER(buffer.left,left)
	BUILD_GPU_BUFFER(buffer.right,right)
#undef BUILD_GPU_BUFFER
/////////////////////////////////////	
}


void SolarSky::loadTextures(
			const Utility::Path& top,
			const Utility::Path& bottom,
			const Utility::Path& front,
			const Utility::Path& back,
			const Utility::Path& left,
			const Utility::Path& right){

	this->top=new Texture(top);		
	this->top->bilinear(true);
	this->top->mipmaps(false);
	
	this->bottom=new Texture(bottom);	
	this->bottom->bilinear(true);
	this->bottom->mipmaps(false);
	
	this->front=new Texture(front);	
	this->front->bilinear(true);
	this->front->mipmaps(false);
	
	this->back=new Texture(back);	
	this->back->bilinear(true);
	this->back->mipmaps(false);
	
	this->left=new Texture(left);	
	this->left->bilinear(true);
	this->left->mipmaps(false);
	
	this->right=new Texture(right);	
	this->right->bilinear(true);
	this->right->mipmaps(false);
}

SolarSky::SolarSky(  SolarRender *render,
				     const Utility::Path& top,
					 const Utility::Path& bottom,
					 const Utility::Path& front,
					 const Utility::Path& back,
					 const Utility::Path& left,
					 const Utility::Path& right)
					:render(render)
					,color(Vector4D::ONE){
	memset(&buffer,0,sizeof(BuffersSkybox));
	loadTextures(top,bottom,front,back,left,right);
	buildSkyBox();

}

SolarSky::SolarSky(SolarRender *render,
					const Table& skybox)
					:render(render)
					,color(Vector4D::ONE){
	memset(&buffer,0,sizeof(BuffersSkybox));

	String pathFilesSkybox=skybox.getTablePath().getDirectory()+"/";
	DEBUG_ASSERT_MSG(skybox.existsAsType("top",Table::STRING)&&
						skybox.existsAsType("bottom",Table::STRING)&&
						skybox.existsAsType("front",Table::STRING)&&
						skybox.existsAsType("back",Table::STRING)&&
						skybox.existsAsType("left",Table::STRING)&&
						skybox.existsAsType("right",Table::STRING),
                        "SolarSky error : not found top, bottom, front, back, left, right in skybox");
	loadTextures(pathFilesSkybox+skybox.getString("top"),
				pathFilesSkybox+skybox.getString("bottom"),
				pathFilesSkybox+skybox.getString("front"),
				pathFilesSkybox+skybox.getString("back"),
				pathFilesSkybox+skybox.getString("left"),
				pathFilesSkybox+skybox.getString("right"));
	//color skybox
	setColor(skybox.getVector4D("color",Vec4(1.0,1.0,1.0,1.0)));	

	buildSkyBox();
}

void SolarSky::draw(Camera& camera){
	
	Mat4 viewmodel=camera.getViewMatrix();
	viewmodel[12]=viewmodel[13]=viewmodel[14]=0.0f;
	glLoadMatrixf(viewmodel);
	render->disableZBuffer();

#define BIND_BUFFER(x)\
	x->bind(0);\
	glBindBuffer( GL_ARRAY_BUFFER, buffer.x );\
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );\
	glVertexPointer  ( 3, GL_FLOAT, sizeof(float)*5, 0  );\
	glNormalPointer  (    GL_FLOAT, sizeof(float)*5, 0 );\
	glTexCoordPointer( 2, GL_FLOAT, sizeof(float)*5,(GLvoid *)( sizeof(float)*3 ));\
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);\
	x->unbind(0);
	//save current color
	Vec4 tmpColor;
	glGetFloatv(GL_CURRENT_COLOR,&tmpColor.x);
	//draw
	glColor4f(color.x,color.y,color.z,color.w);
	BIND_BUFFER(front)
	BIND_BUFFER(back)
	BIND_BUFFER(top)
	BIND_BUFFER(bottom)
	BIND_BUFFER(left)
	BIND_BUFFER(right)
	//reset color
	glColor4f(tmpColor.x,tmpColor.y,tmpColor.z,tmpColor.w);

#undef BIND_BUFFER

	render->enableZBuffer();

}

