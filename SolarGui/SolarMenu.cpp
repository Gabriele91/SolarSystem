#include <stdafx.h>
#include <Application.h>
#include <SolarMenu.h>

///////////////////////
using namespace SolarSystem;
///////////////////////

SolarMenu::SolarMenu(const Table& config){
    //get default values
	sizeBottons=config.getVector2D("size",Vec2(50,50));
	maxSizeBottons=config.getVector2D("maxSize",sizeBottons);
	paddingBottons=config.getVector2D("padding",Vec2(25.0f,10.0f));
	pixelPerSecond=config.getVector2D("velocity",(maxSizeBottons-sizeBottons)*0.5);
	//get table buttons
	if(!config.existsAsType("buttons",Table::TABLE)){		
			DEBUG_ASSERT_MSG(0,"SolarMenu error : "
							   "buttons must to be a table");
	}
	//get buttons
	for(auto val: config.getConstTable("buttons")){
		if(!val.first.isString()){		
			DEBUG_ASSERT_MSG(0,"SolarMenu.buttons error : "
							   "must to be only associative table");
		}
		else
		if(val.second->asType(Table::TABLE)){
			SolarButton* nsb=new SolarButton(val.first.string(), val.second->get<Table>());
			nsb->setScale(Vec3(sizeBottons,1.0));
			buttons.push_back(nsb);
		}
		else{
			DEBUG_ASSERT_MSG(0,"SolarMenu.buttons error : "+
								 (val.first.isString()?
								  val.first.string():
								  String::toString(val.first.integer()))+
								" isn't a table");
		}
	}
	//set position
	Vec2 realsize=sizeBottons+paddingBottons;
	Vec2 realsizeH=realsize*0.5;
	//calc center
	Vec2 menucenter(Application::instance()->getScreen()->getWidth()*0.5,
					realsize.y*0.5);
	//pos buttons
	Vec2 leftPointer(menucenter),
		 rightPointer(menucenter);
	int leftS=buttons.size()/2-1;
	int rightS=buttons.size()/2;
	//cernter
	if(buttons.size()%2){
		buttons[rightS]->setPosition(Vec3(menucenter,0.0));
		leftPointer.x-=realsize.x;
		rightPointer.x+=realsize.x;
		++rightS;
	}
	else{
		leftPointer.x-=realsizeH.x;
		rightPointer.x+=realsizeH.x;
	}
	//left
	for(int x1=leftS;x1>=0;--x1){
		buttons[x1]->setPosition(Vec3(leftPointer,0.0));
		leftPointer.x-=realsize.x;
	}
	//right
	for(int x2=rightS;x2<buttons.size();++x2){
		buttons[x2]->setPosition(Vec3(rightPointer,0.0));
		rightPointer.x+=realsize.x;
	}
}
SolarMenu::~SolarMenu(){
	for(auto button:buttons)
		delete button;
}

bool SolarMenu::addOnClick(const String& name,const std::function<void()>& onClick){
	for(auto button:buttons){
		if(button->isCalled(name)){
			button->addOnClick(onClick);
			return true;
		}
	}
	return false;
}
void SolarMenu::update(float dt){
	for(auto button:buttons)
		button->update(this,dt);
}
void SolarMenu::draw(SolarRender* render){
	//save state
	Vec4 globalViewport;
	SolarRender::BlendState blendState;
	SolarRender::MatrixsState matrixsState;
	blendState=render->getBlendState();
	render->getMatrixsState(matrixsState);	
	glGetFloatv(GL_VIEWPORT,&globalViewport.x);
	render->disableZBuffer();
	//query
	Screen *screen=Application::instance()->getScreen();
	Vec2 windowSize(screen->getWidth(),screen->getHeight());
	//draw objects
	//disable shader
    glUseProgram(0);
	//ortogonal mode (2d)	
    glViewport(0,0,windowSize.x,windowSize.y);
	Matrix4x4 projection;
	projection.setOrtho(0,windowSize.x,0,windowSize.y,0.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);
	glMatrixMode(GL_MODELVIEW);
	//draw
	static const float
	xyUV[]={
			-0.5,-0.5,0.0,0.0,
			-0.5, 0.5,0.0,1.0,
			 0.5,-0.5,1.0,0.0,
			 0.5, 0.5,1.0,1.0
	        };
	//disable vbo
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glDisableClientState(GL_NORMAL_ARRAY);

	for(auto button:buttons){
		//matrix button
		glLoadMatrixf(button->getGlobalMatrix());
		//texture
		button->getCurrentTexture()->bind();
		//set vertex
		glVertexPointer(  2, GL_FLOAT, sizeof(float)*4,  &xyUV[0]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4,  &xyUV[2]);
		//draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	//restore state
    glEnableClientState(GL_NORMAL_ARRAY);
	render->enableZBuffer();
	render->setBlendState(blendState);
	render->setMatrixsState(matrixsState);
    glViewport(globalViewport.x,
			   globalViewport.y,
			   globalViewport.z,
			   globalViewport.w);


}