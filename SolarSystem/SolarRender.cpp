#include <stdafx.h>
#include <SolarRender.h>
#include <Application.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

SolarRender::SolarRender():black(Vec4(0.0,0.0,0.0,1.0),16,16)
						  ,white(Vec4(1.0,1.0,1.0,1.0),16,16)
{
}

void SolarRender::init(){
	//OPENGL
	//view port
	glViewport(0, 0, Application::instance()->getScreen()->getWidth(),
						Application::instance()->getScreen()->getHeight());
	//enable culling
	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	//enable z buffer
	glEnable(GL_DEPTH_TEST);
	//enable texturing	
	glEnable( GL_TEXTURE_1D );
	glEnable( GL_TEXTURE_2D );
	//enable state	
	//always active!
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);       
	//default status for blending    
	glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );   
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}
void SolarRender::setCullFace(SolarRender::CULLFACE type){
	glEnable( GL_CULL_FACE );
	glCullFace( type );
}
void SolarRender::setClearColor(const Vec4& color){	
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

SolarRender::BlendState SolarRender::getBlendState(){	
	SolarRender::BlendState bs;
	//save blend
	glGetIntegerv(GL_BLEND_SRC_RGB , &bs.src);
	glGetIntegerv(GL_BLEND_DST_RGB , &bs.dst);
	bs.enable=glIsEnabled(GL_BLEND)!=false;
	return bs;
}
void SolarRender::setBlendState(const BlendState& bs){	
	//restore old blend state   
	if(!bs.enable) glDisable( GL_BLEND );   
	glBlendFunc( bs.src, bs.dst );
}

void SolarRender::enableLight(){	
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
}
void SolarRender::disableLight(){	
     glDisable(GL_LIGHT0);
     glDisable(GL_LIGHTING);
}
bool SolarRender::lightIsEnable(){	
	GLboolean out;
	glGetBooleanv(GL_LIGHTING,&out);
	return out;
}

void SolarRender::setLight( const Vec3& posiction,
							const Vec4& ambient,
							const Vec4& diffuse,
							const Vec4& specular){	
	 Vec4 pos(posiction,1.0);
     glLightfv(GL_LIGHT0, GL_POSITION, pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
     glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}
void SolarRender::setMaterial(  const Vec4& ambient,
								const Vec4& diffuse,
								const Vec4& specular,
								const Vec4& emission,
								float shininess){
						
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

}