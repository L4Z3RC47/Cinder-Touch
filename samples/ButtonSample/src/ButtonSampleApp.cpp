#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchObjects/Button/BaseButton.h"

#include "cinder/rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

class ButtonSampleApp : public AppNative {
public:
	void setup();
	void update();
	void draw();
	void buttonCallback(touchObject::TouchObjectRef);

	Mouse mMouseConnection;
	ButtonRef mButtonRef;
};

void ButtonSampleApp::setup(){

	mMouseConnection.connect();

	mButtonRef = BaseButton::create(cinder::Vec2f(50.0f, 50.0f), cinder::Vec2f(100.0f, 100.0f), std::bind(&ButtonSampleApp::buttonCallback, this, std::placeholders::_1));
}

void ButtonSampleApp::buttonCallback(touchObject::TouchObjectRef){
	console() << "Button Selected!!!" << endl;
	//Set the button to a random color
	float r = randFloat(0.0f, 1.0f);
	float g = randFloat(0.0f, 1.0f);
	float b = randFloat(0.0f, 1.0f);
	mButtonRef->setObjectColor(ColorA(r, g, b));

}
void ButtonSampleApp::update(){
	TouchManager::getInstance()->update();
}

void ButtonSampleApp::draw(){
	// clear out the window with black
	gl::clear(Color(0, 0, 0));

	//draw the button
	mButtonRef->draw();

	//draw the touches. The ID of the mouse will always be -1.
	TouchManager::getInstance()->draw();

}

CINDER_APP_NATIVE(ButtonSampleApp, RendererGl)
