//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: To test TUIO input, we've created a button the size of the screen. Touches will draw & be labeled with incrementing touchIDs. 
//----------------------------------------------------------------------------
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"			//include touch manager
#include "TouchDrivers/Tuio.h"		//include the driver you want to use (options are Mouse, TUIO)
#include "TouchObjects/BaseButton.h"//include baseButton

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject; //use the namespace for easier access

class TUIOTestSampleApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
	void buttonCallback(TouchObjectRef);

private:
	std::shared_ptr<TouchManager>	mTouchManager;	//keep an instance of the touch manager
	Tuio							mTuioConnection;//connection that will be made to the driver
	ButtonRef						mButtonRef;		//shared pointer to button
};

void TUIOTestSampleApp::setup(){
	//connect the driver that was included, in this case TUIO
	mTuioConnection.connect();
	//get an instance of the touch manager
	mTouchManager = TouchManager::getInstance();

	//create a new shared pointer to a button and setup the button settings
	mButtonRef = BaseButton::create(cinder::Vec2f::zero(), cinder::Vec2f(getWindowWidth(), getWindowHeight()), std::bind(&TUIOTestSampleApp::buttonCallback, this, std::placeholders::_1));
}

void TUIOTestSampleApp::buttonCallback(TouchObjectRef){
	console() << "TUIOTestSampleApp::buttonCallback" << endl;
}

void TUIOTestSampleApp::update(){
	//we'll always update the TouchManager, to make sure all touches are on the main thread
	mTouchManager->update();
}

void TUIOTestSampleApp::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	//draw the button
	gl::color(1.0f, 1.0f, 0.5f);
	mButtonRef->draw();

	//draw the touches
	mTouchManager->draw();
}

CINDER_APP_NATIVE( TUIOTestSampleApp, RendererGl )
