#include "Tuio.h"

using namespace std;
using namespace ci;
using namespace ci::app;

Tuio::Tuio(){
	mTouchManagerPntr	= NULL;
	//shared pointer to the Touch Manager
	mTouchManagerPntr = TouchManager::getInstance();
}

void Tuio::connect(){
	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	//Touch Events
	mTuioConnection.connect();
    mTuioTouchBeganCallBack = mTuioConnection.registerTouchesBegan(this, &Tuio::tuioTouchesBegan);
    mTuioTouchMovedCallBack = mTuioConnection.registerTouchesMoved(this, &Tuio::tuioTouchesMoved);
	mTuioTouchEndedCallBack = mTuioConnection.registerTouchesEnded(this, &Tuio::tuioTouchesEnded);

	mTuioObjectBeganCallBack = mTuioConnection.registerObjectAdded(this, &Tuio::tuioObjectBegan);
	mTuioObjectMovedCallBack = mTuioConnection.registerObjectUpdated(this, &Tuio::tuioObjectMoved);
	mTuioObjectEndedCallBack = mTuioConnection.registerObjectRemoved(this, &Tuio::tuioObjectEnded);
}

Tuio::~Tuio(){
	//disconnect();
}

void Tuio::disconnect(){
	//Disconnect from the TUIO TOUCH signals
	try{
		mTuioConnection.unregisterTouchesBegan(mTuioTouchBeganCallBack);
		mTuioConnection.unregisterTouchesMoved(mTuioTouchMovedCallBack);
		mTuioConnection.unregisterTouchesEnded(mTuioTouchEndedCallBack);

		mTuioConnection.unregisterObjectAdded(mTuioObjectBeganCallBack);
		mTuioConnection.unregisterObjectUpdated(mTuioObjectMovedCallBack);
		mTuioConnection.unregisterObjectRemoved(mTuioObjectEndedCallBack);
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
		mTouchManagerPntr->touchEvent(event.getFiducialId(), event.getTouch(getElapsedSeconds(), Vec2f(getWindowWidth(), getWindowHeight())).getPos(), touchObject::TouchType::OBJECT, TouchManager::BEGAN);
}

void Tuio::tuioObjectMoved(cinder::tuio::Object event){
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(event.getFiducialId(), event.getTouch(getElapsedSeconds(), Vec2f(getWindowWidth(), getWindowHeight())).getPos(), touchObject::TouchType::OBJECT, TouchManager::MOVED);
}

void Tuio::tuioObjectEnded(cinder::tuio::Object event){
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(event.getFiducialId(), event.getTouch(getElapsedSeconds(), Vec2f(getWindowWidth(), getWindowHeight())).getPos(), touchObject::TouchType::OBJECT, TouchManager::ENDED);
}