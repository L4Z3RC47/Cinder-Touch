#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/rand.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchObjects/BaseButton.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

class Button_vc2013App : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;

	void buttonConnection(touchObject::TouchObjectRef);

	Mouse mMouseConnection;
	ButtonRef mButtonRef;
};

void Button_vc2013App::setup(){
	mMouseConnection.connect();

	mButtonRef = BaseButton::create(ci::vec2(50.0f, 50.0f), ci::vec2(100.0f, 100.0f), std::bind(&Button_vc2013App::buttonConnection, this, std::placeholders::_1));
}

void Button_vc2013App::update(){
	TouchManager::getInstance()->update();
}

void Button_vc2013App::buttonConnection(touchObject::TouchObjectRef){
	console() << "Button Selected!!!" << endl;
	//Set the button to a random color
	float r = randFloat(0.0f, 1.0f);
	float g = randFloat(0.0f, 1.0f);
	float b = randFloat(0.0f, 1.0f);
	mButtonRef->setObjectColor(ColorA(r, g, b));
}

void Button_vc2013App::draw(){
	gl::clear( Color( 0, 0, 0 ) ); 
	
	//draw the button
	mButtonRef->draw();

	//draw the touches. The ID of the mouse will always be -1.
	TouchManager::getInstance()->draw();
}

CINDER_APP( Button_vc2013App, RendererGl )
