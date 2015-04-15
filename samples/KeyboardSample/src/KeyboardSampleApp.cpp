#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchDrivers/TUIO.h"

#include "TouchObjects/Keyboard/Key.h"
#include "TouchObjects/Keyboard/Keyboard.h"

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
	void addKeys();

	Tuio	mTuioConnection;
	Mouse	mMouseConnection;
	void keyboardSubmitFn(std::string);

	KeyboardRef keyboardRef;
};

void KeyboardSampleApp::setup()
{
	gl::enableAlphaBlending();
	mTuioConnection.connect();
	mMouseConnection.connect();
	keyboardRef = Keyboard::create(Vec2f(20, 20), Vec2f(210, 500));
	keyboardRef->setSubmitFunction(std::bind(&KeyboardSampleApp::keyboardSubmitFn, this, std::placeholders::_1));
	keyboardRef->setInputFieldColor(ci::ColorA(44.0f / 255.0f, 251.0f / 255.0f, 232.0f / 255.0f,1));
	keyboardRef->setInputFieldFont(ci::Font("arial", 20.0f));
	addKeys();

}
void KeyboardSampleApp::addKeys(){

		float buttonWidth = 50.0f;
		float buttonHeight = 50.0f;
		float verticalPadding = 100.0f;
		float horizontalPadding = 25.0f;

		float verticalSpacing = 75.0f;
		float horizontalSpacing = 75.0f;
		float yPos = verticalPadding;
		float xPos = horizontalPadding;
		float buttonsPerRow = 3;


		int buttonsOnRow = 0;
		for (int i = 1; i <= 10; i++){
			if (buttonsOnRow == buttonsPerRow){
				buttonsOnRow = 0;
				yPos += verticalSpacing;
				xPos = horizontalPadding;
			}

			if (i < 10){
				KeyRef keyRef = Key::create(Vec2f(xPos, yPos), Vec2f(buttonWidth, buttonHeight), to_string(i));
				keyboardRef->addKey(keyRef);
				xPos += horizontalSpacing;
				buttonsOnRow++;
			}
			if (i == 10){//Add the bottom number 0 in the center of the row
				KeyRef keyRef = Key::create(Vec2f(horizontalPadding + horizontalSpacing, yPos), Vec2f(buttonWidth, buttonHeight), "0");
				keyboardRef->addKey(keyRef);
			}

		}



		KeyRef clearKeyRef = Key::create(Vec2f(horizontalPadding, yPos), Vec2f(50, 50), "Clear");
		clearKeyRef->setKeyType(touchObject::KeyType::Clear);
		keyboardRef->addKey(clearKeyRef);

		KeyRef bkspcKeyRef = Key::create(Vec2f((2 * buttonWidth) + horizontalSpacing, yPos), Vec2f(50, 50), "DEL");
		bkspcKeyRef->setKeyType(touchObject::KeyType::Backspace);
		keyboardRef->addKey(bkspcKeyRef);


		KeyRef submitKeyRef = Key::create(Vec2f(horizontalPadding + buttonWidth, yPos + verticalSpacing), Vec2f(2 * buttonWidth, buttonHeight), "Submit");
		submitKeyRef->setKeyType(touchObject::KeyType::Submit);
		keyboardRef->addKey(submitKeyRef);

}
void KeyboardSampleApp::mouseDown( MouseEvent event )
{
}

void KeyboardSampleApp::keyboardSubmitFn(std::string inputString){
	console() << " Keyboard Submitted : " << inputString << endl;
}

void KeyboardSampleApp::update()
{
	TouchManager::getInstance()->update();
}

void KeyboardSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	

	keyboardRef->draw();

	//draw the touches. The ID of the mouse will always be -1.
	TouchManager::getInstance()->draw();
}

CINDER_APP_NATIVE( KeyboardSampleApp, RendererGl )
