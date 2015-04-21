//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: This sample demonstrates how to nest multiple objects that accept touches.
//----------------------------------------------------------------------------
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"		//include touch manager
#include "TouchDrivers/Mouse.h" //include the driver you want to use (options are Mouse, TUIO)

#include "SampleObject.h"		//include the sample object header

using namespace ci;
using namespace ci::app;
using namespace std;

class MultipleTouchObjectsApp : public AppNative {
  public:
	void setup();
	void addObjects();
	void update();
	void draw();

  private:
	//connection that will be made to the driver
	Mouse								mMouseConnection;
	//keep an instance of the touch manager
	std::shared_ptr<TouchManager>		mTouchManager;
	//the main obj
	std::shared_ptr<SampleObject>		mGreenObject;
};

void MultipleTouchObjectsApp::setup(){
	//connect the driver that was included, in this case the mouse
	mMouseConnection.connect();
	//get an instance of the touch manager
	mTouchManager = TouchManager::getInstance();
	//add all of the objects, which we will make touchable
	addObjects();
}

void MultipleTouchObjectsApp::addObjects(){
	//in this sample, we are going to nest objects within each other. the green object is the parent
	mGreenObject = std::shared_ptr<class SampleObject>(new SampleObject());
	mGreenObject->setup(Vec2f(50.0f, 50.0f), Vec2f(100.0f, 100.0f), ColorA(0.0f, 1.0f, 0.0f, 1.0f));

	//add 3 red objects to the green object
	for (int i = 0; i < 3; i++){
		shared_ptr< class SampleObject > redObject = shared_ptr<class SampleObject>(new SampleObject());

		//add this redObject to the vector of all the greenObject's children
		mGreenObject->mChildVector.push_back(redObject);
		
		redObject->setup(Vec2f(100.0f, i*150.0f), Vec2f(50.0f, 50.0f), ColorA(1.0f, 0.0f, 0.0f, 1.0f));
		//Need to tell the red object about the Green object's position so we can detect touches
		console() << "Green Global Pos " << mGreenObject->getGlobalPosition().x << " , " << mGreenObject->getGlobalPosition().y << endl;
		redObject->setParentPosition(mGreenObject->getGlobalPosition() );
		
		//add 3 blue objects to each red object
		for (int j = 0; j < 3; j++){
			shared_ptr< class SampleObject > blueObject = shared_ptr<class SampleObject>(new SampleObject());
			//add this blueObject to the vector of all the redObject's children
			redObject->mChildVector.push_back(blueObject);
			blueObject->setup(Vec2f(100.0f, j*50.0f), Vec2f(25.0f, 20.0f), ColorA(0.0f, 0.0f, 1.0f, 1.0f));
			//Need to tell the blue object about the ReD object's position so we can detect touches
		
			console() << "redObject Global Pos " << redObject->getGlobalPosition().x << " , " << redObject->getGlobalPosition().y << endl;
			blueObject->setParentPosition(redObject->getGlobalPosition());

		}
		
	}
	
}

void MultipleTouchObjectsApp::update(){
	//we'll always update the TouchManager, to make sure all touches are on the main thread
	mTouchManager->update();
}

void MultipleTouchObjectsApp::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	//draw the green object
	mGreenObject->draw();
}

CINDER_APP_NATIVE( MultipleTouchObjectsApp, RendererGl )
