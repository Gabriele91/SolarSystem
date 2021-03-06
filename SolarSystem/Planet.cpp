#include <stdafx.h>
#include <Planet.h>
#include <Debug.h>

///////////////////////
using namespace SolarSystem;
///////////////////////
//SHADER
Planet::SunLightALL        Planet::sunLightALL;
Planet::SunLight           Planet::sunLight;
Planet::SunLightCloud      Planet::sunLightCloud;
Planet::SunLightAtmosphere Planet::sunLightAtmosphere;
Planet::SunLightRings      Planet::sunLightRings;
void Planet::initShader()
{
    //all effects in one
	sunLightALL.shader.loadShader("shader/sunLightALL.vs","shader/sunLightALL.ps");
	sunLightALL.glPlanetTexture=sunLightALL.shader.getUniformID("planetTexture");
	sunLightALL.glPlanetNightTexture=sunLightALL.shader.getUniformID("planetNightTexture");
	sunLightALL.glPlanetSpecularTexture=sunLightALL.shader.getUniformID("planetSpecularTexture");
	sunLightALL.glCloudTexture=sunLightALL.shader.getUniformID("cloudTexture");
	sunLightALL.glAtmGrad1=sunLightALL.shader.getUniformID("atmGrad1");
	sunLightALL.glAtmGrad2=sunLightALL.shader.getUniformID("atmGrad2");	
	sunLightALL.atmRim=sunLightALL.shader.getUniformID("atmRim");
	sunLightALL.textureMat4=sunLightALL.shader.getUniformID("textureMat4");	
    //load shader sun light (planets):
	sunLight.shader.loadShader("shader/sunLight.vs","shader/sunLight.ps");
	sunLight.glPlanetTexture=sunLight.shader.getUniformID("planetTexture");
	sunLight.glPlanetNightTexture=sunLight.shader.getUniformID("planetNightTexture");
	sunLight.glPlanetSpecularTexture=sunLight.shader.getUniformID("planetSpecularTexture");
	//load shader sun light (clouds):
	sunLightCloud.shader.loadShader("shader/sunLightClouds.vs","shader/sunLightClouds.ps");
	sunLightCloud.glCloudTexture=sunLightCloud.shader.getUniformID("cloudTexture");
	sunLightCloud.textureMat4=sunLightCloud.shader.getUniformID("textureMat4");	
	//shader  light  on rings
	sunLightRings.shader.loadShader("shader/sunLightRings.vs","shader/sunLightRings.ps");
	sunLightRings.glRingsTexture=sunLightRings.shader.getUniformID("ringsTexture");
	//load shader sun light (atmospheres):
	sunLightAtmosphere.shader.loadShader("shader/sunLightAtmosphere.vs","shader/sunLightAtmosphere.ps");
	sunLightAtmosphere.glAtmGrad1=sunLightAtmosphere.shader.getUniformID("atmGrad1");
	sunLightAtmosphere.glAtmGrad2=sunLightAtmosphere.shader.getUniformID("atmGrad2");	
	sunLightAtmosphere.atmRim=sunLightAtmosphere.shader.getUniformID("atmRim");	
}        
///////////////////////
void Planet::buildMesh(int rings,int sectors){
	// make planet mesh 
	verticesSize=rings*sectors;
	indicesSize=(rings-1)*(sectors-1)*6;
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
			vertices[idv].texcoords.x=1.0f-s*S;
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
			  ,specularTexture(NULL)
			  ,atmGrad1(NULL)
			  ,atmGrad2(NULL)
			  ,atmRim(NULL)
			  ,rings(NULL)
			  ,render(render)
			  ,rotationNormal(0.0f,1.0f,0.0f)
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
	if(atmGrad1) 
		delete atmGrad1;
	if(atmGrad2) 
		delete atmGrad2;
	if(atmRim) 
		delete atmRim;
	//if(rings)
		//delete rings;
}
//set extra texture
void Planet::setCloudTexture(const Utility::Path& argtexture){
	DEBUG_ASSERT(cloudTexture==NULL);
	cloudTexture=new Texture(argtexture);
}
void Planet::setBlackTexture(const Utility::Path& argtexture){
	DEBUG_ASSERT(blackTexture==NULL);
	blackTexture=new Texture(argtexture);
};
void Planet::setSpecularTexture(const Utility::Path& argtexture){
	DEBUG_ASSERT(specularTexture==NULL);
	specularTexture=new Texture(argtexture);
}
void Planet::setAtmosphereTexture(const Utility::Path& grad1,
								  const Utility::Path& grad2,
								  const Utility::Path& rim){
		DEBUG_ASSERT(atmGrad1==NULL);
		DEBUG_ASSERT(atmGrad2==NULL);
		DEBUG_ASSERT(atmRim==NULL);
		atmGrad1=new Texture1D(grad1);
		atmGrad2=new Texture1D(grad2);
		atmRim=new Texture1D(rim);
}
void Planet::setRings(const Utility::Path& argtexture,
					  float nr,
					  float fr,
					  const Vec3& rotationOffSet){
	DEBUG_ASSERT(rings==NULL);
	rings=new SolarRings(render,argtexture,nr,fr);
	Quaternion q;
	q.setFromEulero(rotationOffSet.x,rotationOffSet.y,rotationOffSet.z);
	rings->setRotation(q);
	addChild(rings,Object::ENABLE_ALL,true);
}


void Planet::uniformCloudOffset(Shader& shader,uint id)
{
	if((cloudDayOffset.x+cloudDayOffset.y+cloudDayOffset.z)!=0.0f)
    {
        Mat4 uvmat;
        uvmat.setRotZ(cloudDayOffset.y);
        uvmat[12]=cloudDayOffset.x;
        uvmat[13]=cloudDayOffset.z;
        shader.uniformMatrix4x4(id,uvmat);
    }
}
//draw
void Planet::draw(Camera& camera){
	drawPlanet(camera);
	drawCloud(camera);
}
bool Planet::inCamera(Camera& camera){
	const Mat4& thisMtx=getGlobalMatrix();
	const Vec3& thisScale=thisMtx.getScale3D();
	const float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z));
	//culling
	return camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE;	
}
void Planet::drawSphere(){
		bindMesh();
}
void Planet::drawCircle(){		
	if(rings){
		rings->drawRing();
	}
}
void Planet::drawBase(Camera& camera){
	////////////////////////////////////
	//get values
	const Mat4& thisMtx=getGlobalMatrix();
	const Vec3& thisScale=thisMtx.getScale3D();
	const float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z));
	//culling
	if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE)
    {
		//set model matrix
		Mat4 viewmodel=camera.getViewMatrix().mul(getGlobalMatrix());
		glLoadMatrixf(viewmodel);
		//draw
		bindMesh();
    }
}
void Planet::drawBaseRings(Camera& camera){	
	if(rings){
		rings->drawBase(camera);
	}
}
/////////////
void Planet::drawPlanet(Camera& camera){
	////////////////////////////////////
	//get values
	const Mat4& thisMtx=getGlobalMatrix();
	const Vec3& thisScale=thisMtx.getScale3D();
	const float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z));
	//culling
	if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE) {
		if(render->lightIsEnable()){	
				render->setMaterial(ambient,diffuse,specular,emission,shininess);			
				
				if(blackTexture) blackTexture->bind(1);
				else render->getTextureBlack().bind(1);	

				if(specularTexture) specularTexture->bind(2);
				else render->getTextureWhite().bind(2);
			}
		//bind texture 
		texture.bind(0);
		//set model matrix
		Mat4 viewmodel=camera.getViewMatrix().mul(thisMtx);
		glLoadMatrixf(viewmodel);
		//draw
		bindMesh();
		//unbind texture 
		texture.unbind(0);
		//unbind
		if(render->lightIsEnable()){			
			if(blackTexture) blackTexture->unbind(1);
			else render->getTextureBlack().unbind(1);			
			
			if(specularTexture) specularTexture->unbind(2);
			else render->getTextureWhite().unbind(2);
		}
	}
}
void Planet::drawCloud(Camera& camera){	
	if(cloudTexture){
		////////////////////////////////////
		//get values
		Mat4 thisMtx=getGlobalMatrix();
		const Vec3& thisScale=thisMtx.getScale3D();
		//z-buffer fixed
		float distToCam=camera.getPosition(true).distance(getPosition(true));
		float scaleCloud=distToCam < 600 ? 1.003f : ( distToCam<1200 ? 1.015f : 1.03f );
		//z-buffer fixed
		float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z))*scaleCloud;
		//culling
		if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE) {
				//save blend
				auto stateBlend=render->getBlendState();
				//set additive blend
				if(!stateBlend.enable) glEnable( GL_BLEND );   
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA  );				
                //set matrix
				Mat4 viewmodel=camera.getViewMatrix().mul(thisMtx);
				viewmodel.addScale(Vec3(scaleCloud,scaleCloud,scaleCloud));
				glLoadMatrixf(viewmodel);
				//set material
				if(render->lightIsEnable())
					render->setMaterial(ambient,diffuse,specular,emission,shininess);	
				//bind texture
				cloudTexture->bind(0);
				//draw
				bindMesh();
				//bind texture
				cloudTexture->unbind(0);
				//reset old blend state  
				render->setBlendState(stateBlend);
		}
	}
}
void Planet::drawAtmosphere(Camera& camera){	
	if(atmGrad1&&atmGrad2&&atmRim){
		////////////////////////////////////
		//get values
		const Mat4& thisMtx=getGlobalMatrix();
		const Vec3& thisScale=thisMtx.getScale3D();		
		//z-buffer fixed
		float distToCam=camera.getPosition(true).distance(getPosition(true));
		float scaleAtmosphere=distToCam<600 ? 1.006f : ( distToCam<1200 ? 1.03f : 1.06f );
		//z-buffer fixed
		float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z))*scaleAtmosphere;
		//culling
		if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE) {
				//save blend
				auto stateBlend=render->getBlendState();
				//set additive blend
				if(!stateBlend.enable) glEnable( GL_BLEND );   
				glBlendFunc( GL_SRC_ALPHA, GL_ONE );				
				//set model matrix
				Mat4 viewmodel=camera.getViewMatrix().mul(thisMtx);
				viewmodel.addScale(Vec3(scaleAtmosphere,scaleAtmosphere,scaleAtmosphere));
				glLoadMatrixf(viewmodel);
				//bind textures
				atmGrad1->bind(0);
				atmGrad2->bind(1);
				atmRim->bind(2);
				//draw
				bindMesh();
				//unbind textures
				atmGrad1->unbind(0);
				atmGrad2->unbind(1);
				atmRim->unbind(2);
				//reset old blend state  
				render->setBlendState(stateBlend);
		}
	}
}
void Planet::drawPlanetALL(Camera& camera)
{	
    ////////////////////////////////////
	//get values
	const Mat4& thisMtx=getGlobalMatrix();
	const Vec3& thisScale=thisMtx.getScale3D();
	const float thisMaxScale=Math::max(thisScale.x,Math::max(thisScale.y,thisScale.z));
	//culling
	if(camera.sphereInFrustum(thisMtx.getTranslation3D(),thisMaxScale) != Camera::OUTSIDE) 
    {	
        //set material
		render->setMaterial(ambient,diffuse,specular,emission,shininess);
        
        //day texture
		texture.bind(0);
        
        //night texture
		if(blackTexture) blackTexture->bind(1);
		else render->getTextureBlack().bind(1);	

        //specular texture
		if(specularTexture) specularTexture->bind(2);
		else render->getTextureWhite().bind(2);

        //clouds
		if(cloudTexture) cloudTexture->bind(3);
        else render->getTextureBlackAlpha0().bind(3);

        //atmosfere
		if(atmGrad1) atmGrad1->bind(4);
        else render->getTextureBlackAlpha0().bind(4);
		if(atmGrad2) atmGrad2->bind(5);
        else render->getTextureBlackAlpha0().bind(5);
		if(atmRim) atmRim->bind(6);
        else render->getTextureWhite().bind(6);
        
		//set model matrix
		Mat4 viewmodel=camera.getViewMatrix().mul(thisMtx);
		glLoadMatrixf(viewmodel);
		//draw
		bindMesh();

		//unbind textures
		texture.unbind(0);

		if(blackTexture) blackTexture->unbind(1);
		else render->getTextureBlack().unbind(1);	

		if(specularTexture) specularTexture->unbind(2);
		else render->getTextureWhite().unbind(2);

		if(cloudTexture) cloudTexture->unbind(3);
        else render->getTextureBlackAlpha0().unbind(3);

		if(atmGrad1) atmGrad1->unbind(4);
        else render->getTextureBlackAlpha0().unbind(4);

		if(atmGrad2) atmGrad2->unbind(5);
        else render->getTextureBlackAlpha0().unbind(5);

		if(atmRim) atmRim->unbind(6);
        else render->getTextureWhite().unbind(6);
    }
}
/////////////
void Planet::drawRings(Camera& camera){
	if(rings){
		rings->draw(camera);
	}
}
/////////////
void Planet::drawPlanetAuto(Camera& camera)
{
	float distToCam=camera.getPosition(true).distance(getPosition(true));
	bool  drawParts=distToCam < 600 ;
    bool  isOnlyPlanet = !(atmGrad1&&atmGrad2&&atmRim || cloudTexture);
    //select...
    if(drawParts || isOnlyPlanet)
    {
	    //draw planet
        Planet::sunLight.shader.bind();
	    Planet::sunLight.uniforming();	
		    drawPlanet(camera);	
	    Planet::sunLight.shader.unbind();
	    //draw cloud
        if(cloudTexture)
        {
	        Planet::sunLightCloud.shader.bind();
	        Planet::sunLightCloud.uniforming();	
                uniformCloudOffset(sunLightCloud.shader,sunLightCloud.textureMat4);
		        drawCloud(camera);
	        Planet::sunLightCloud.shader.unbind();	
        }
	    //draw atmosphere
        if(atmGrad1&&atmGrad2&&atmRim)
        {
            Planet::sunLightAtmosphere.shader.bind();
	        Planet::sunLightAtmosphere.uniforming();	
	            drawAtmosphere(camera);
	        Planet::sunLightAtmosphere.shader.unbind();
        }
    }
    else
    {
        Planet::sunLightALL.shader.bind();
	    Planet::sunLightALL.uniforming();	
            uniformCloudOffset(sunLightALL.shader,sunLightALL.textureMat4);
            drawPlanetALL(camera);
	    Planet::sunLightALL.shader.unbind();
    }

}

//set data
void Planet::setData(float _day){
	//save
	day=_day;
	if(daysOfYear!=0.0f){
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
	}
	else{
		setPosition(Vec3(radius.x,0.0,radius.y));
	}
	if(rotationPeriod!=0.0f){
		//calc rotation
		float rotationDay=day/rotationPeriod*Math::PI2;
		Quaternion rot;
		Vec3 roation=rotationNormal*rotationDay;
		rot.setFromEulero(roation.x,roation.y,roation.z);
		setRotation(rot);
		//calc offset
		if((cloudOffset.x+cloudOffset.y+cloudOffset.z)!=0.0f)
			cloudDayOffset=cloudOffset*rotationDay;
	}
}
//set planet info
void Planet::setPlanetInfo(const Vec2& _radius, 
						   float _daysOfYear,
						   float _rotationPeriod){
	radius=_radius;
	daysOfYear=_daysOfYear;
	rotationPeriod=_rotationPeriod;
}
//cloud info
void Planet::setCloudOffset(const Vec3& _cloudOffset){
	cloudOffset=_cloudOffset;
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
