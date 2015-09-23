#include "Tuio.h"

using namespace std;
using namespace ci;
using namespace ci::app;

Tuio::Tuio(){
	mTouchManagerPntr	= NULL;
	//shared pointer to the Touch Manager
	mTouchManagerPntr = TouchManager::getInstance();
}

Tuio::~Tuio(){
	mTuioConnection.disconnect();
}

void Tuio::connect(){
	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	//Touch Events
	mTuioConnection.connect();
    mTuioTouchBeganConnection = mTuioConnection.registerTouchesBegan(this, &Tuio::tuioTouchesBegan);
	mTuioTouchMovedConnection = mTuioConnection.registerTouchesMoved(this, &Tuio::tuioTouchesMoved);
	mTuioTouchEndedConnection  = mTuioConnection.registerTouchesEnded(this, &Tuio::tuioTouchesEnded);

	mTuioObjectBeganConnection = mTuioConnection.registerObjectAdded(this, &Tuio::tuioObjectBegan);
	mTuioObjectMovedConnection = mTuioConnection.registerObjectUpdated(this, &Tuio::tuioObjectMoved);
	mTuioObjectEndedConnection = mTuioConnection.registerObjectRemoved(this, &Tuio::tuioObjectEnded);
}

void Tuio::disconnect(){
	//Disconnect from the TUIO TOUCH signals
	try{
		mTuioConnection.unregisterTouchesBegan(mTuioTouchBeganConnection);
		mTuioConnection.unregisterTouchesMoved(mTuioTouchMovedConnection);
		mTuioConnection.unregisterTouchesEnded(mTuioTouchEndedConnection);

		mTuioConnection.unregisterObjectAdded(mTuioObjectBeganConnection);
		mTuioConnection.unregisterObjectUpdated(mTuioObjectMovedConnection);
		mTuioConnection.unregisterObjectRemoved(mTuioObjectEndedConnection);
	}catch(...){
		console() << "Couldnt disconnect TUIO From the touch manager. Touch manager may have already been deleted." << endl;
	}

	//Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Tuio::tuioTouchesBegan(cinder::app::TouchEvent event){
	if (mTouchManagerPntr){
		for (TouchEvent::Touch touch : event.getTouches())
			mTouchManagerPntr->touchEvent(touch.getId(), touch.getPos(), touchObject::TouchType::TOUCH, TouchManager::BEGAN);
	}
}

void Tuio::tuioTouchesMoved(cinder::app::TouchEvent event){
	if (mTouchManagerPntr){
		for (TouchEvent::Touch touch : event.getTouches())
			mTouchManagerPntr->touchEvent(touch.getId(), touch.getPos(), touchObject::TouchType::TOUCH, TouchManager::MOVED);
	}
}

void Tuio::tuioTouchesEnded(cinder::app::TouchEvent event){
	if (mTouchManagerPntr){
		for (TouchEvent::Touch touch : event.getTouches())
			mTouchManagerPntr->touchEvent(touch.getId(), touch.getPos(), touchObject::TouchType::TOUCH, TouchManager::ENDED);
	}
}

void Tuio::tuioObjectBegan(cinder::tuio::Object event){
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(event.getFiducialId(), event.getTouch(getElapsedSeconds(), vec2(getWindowWidth(), getWindowHeight())).getPos(), touchObject::TouchType::OBJECT, TouchManager::BEGAN);
}

void Tuio::tuioObjectMoved(cinder::tuio::Object event){
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(event.getFiducialId(), event.getTouch(getElapsedSeconds(), vec2(getWindowWidth(), getWindowHeight())).getPos(), touchObject::TouchType::OBJECT, TouchManager::MOVED);
}

void Tuio::tuioObjectEnded(cinder::tuio::Object event){
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(event.getFiducialId(), event.getTouch(getElapsedSeconds(), vec2(getWindowWidth(), getWindowHeight())).getPos(), touchObject::TouchType::OBJECT, TouchManager::ENDED);
}