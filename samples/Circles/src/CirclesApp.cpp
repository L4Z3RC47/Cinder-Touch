//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2015
//	Developers: Stacey Martens
//  Contents: 
//  Comments: 

//translate
//scale
//scene graph

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
	std::shared_ptr<TouchManager>	mTouchManager;	//keep an instance of the touch manager
	Mouse							mMouseConnection;//connection that will be made to the driver
	std::shared_ptr<RedObj>			mRedCircle;

};

void CirclesApp::setup(){
	gl::enableAlphaBlending();

	//connect the driver that was included, in this case TUIO
	mMouseConnection.connect();
	//get an instance of the touch manager
	mTouchManager = TouchManager::getInstance();

	//create main red circle
	mRedCircle = std::shared_ptr<class RedObj>(new RedObj());
	mRedCircle->setup();
}

void CirclesApp::mouseDown( MouseEvent event ){
}

void CirclesApp::update(){
	//we'll always update the TouchManager, to make sure all touches are on the main thread
	mTouchManager->update();
}

void CirclesApp::draw(){
	gl::clear( Color( 0, 0, 0 ) ); 
	mRedCircle->draw();
}

CINDER_APP( CirclesApp, RendererGl )
