//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2015
//	Developers: Stacey Martens
//  Contents: Main class contains one red circle, which contains one blue circle. 
//  Comments: Sample showing how to setup touchable circles, and also what you need to do 
//			to translate positions from the parent down to the child. 
//----------------------------------------------------------------------------
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"

#include "RedObj.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CirclesApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
	//! Reference to the single instance of the touch manager
	std::shared_ptr<TouchManager>	mTouchManager;
	//! Connection that will be made to the driver
	Mouse							mMouseConnection;
	std::shared_ptr<RedObj>			mRedCircle;

};

void CirclesApp::setup(){
	gl::enableAlphaBlending();

	//Connect the driver that was included (in this case mouse)
	mMouseConnection.connect();

	//Get an instance of the touch manager
	mTouchManager = TouchManager::getInstance();

	//Create main red circle
	mRedCircle = std::shared_ptr<class RedObj>(new RedObj());
	mRedCircle->setup();
}

void CirclesApp::mouseDown( MouseEvent event ){
}

void CirclesApp::update(){
	//Always need to update the TouchManager. This ensures all touches are on the main thread.
	mTouchManager->update();
}

void CirclesApp::draw(){
	gl::clear( Color( 0, 0, 0 ) ); 
	mRedCircle->draw();
}

CINDER_APP( CirclesApp, RendererGl )