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

class SimpleButtonApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;

	void buttonConnection(touchObject::TouchObjectRef);

	Mouse mMouseConnection;
	ButtonRef mButtonRef;
};

void SimpleButtonApp::setup(){
	mMouseConnection.connect();

	mButtonRef = BaseButton::create(ci::vec2(50.0f, 50.0f), ci::vec2(100.0f, 100.0f), std::bind(&SimpleButtonApp::buttonConnection, this, std::placeholders::_1));
}

void SimpleButtonApp::update(){
	TouchManager::getInstance()->update();
}

void SimpleButtonApp::buttonConnection(touchObject::TouchObjectRef){
	console() << "Button Selected!!!" << endl;
	//Set the button to a random color
	float r = randFloat(0.0f, 1.0f);
	float g = randFloat(0.0f, 1.0f);
	float b = randFloat(0.0f, 1.0f);
	mButtonRef->setObjectColor(ColorA(r, g, b));
}

void SimpleButtonApp::draw(){
	gl::clear(Color(0, 0, 0));

	//draw the button
	mButtonRef->draw();

	//draw the touches. The ID of the mouse will always be -1.
	TouchManager::getInstance()->draw();
}

CINDER_APP( SimpleButtonApp, RendererGl )
