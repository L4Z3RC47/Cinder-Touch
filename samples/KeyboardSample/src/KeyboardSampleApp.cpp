#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchDrivers/TUIO.h"
//#include "TouchObjects/Button/BaseButton.h"
//#include "TouchObjects/Keyboard/Key.h"
//#include "TouchObjects/Keyboard/Keyboard.h"
#include "PhoneNumberKeyboard.h"
using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

class KeyboardSampleApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void buttonCallback(touchObject::TouchObjectRef);
	Tuio	mTuioConnection;
	Mouse	mMouseConnection;
	void keyboardSubmitFn(std::string);
	PhoneNumberKeyboardRef phoneNumberKeyboardRef;
};

void KeyboardSampleApp::setup()
{
	gl::enableAlphaBlending();
	mTuioConnection.connect();
	mMouseConnection.connect();
	
	phoneNumberKeyboardRef = PhoneNumberKeyboard::create(Vec2f(10, 10), Vec2f(300, 500));
	phoneNumberKeyboardRef->setSubmitFunction(std::bind(&KeyboardSampleApp::keyboardSubmitFn, this, std::placeholders::_1));


}

void KeyboardSampleApp::mouseDown( MouseEvent event )
{
}
void KeyboardSampleApp::keyboardSubmitFn(std::string phoneNumber){
	console() << "PHONE NUMBER SUBMITTED : " << phoneNumber << endl;

}

void KeyboardSampleApp::update()
{
	TouchManager::getInstance()->update();
}

void KeyboardSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	

	phoneNumberKeyboardRef->draw();

	//draw the touches. The ID of the mouse will always be -1.
	TouchManager::getInstance()->draw();
}

CINDER_APP_NATIVE( KeyboardSampleApp, RendererGl )
