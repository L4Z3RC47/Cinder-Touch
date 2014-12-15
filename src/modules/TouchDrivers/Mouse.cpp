#include "Mouse.h"

using namespace ci::app;

Mouse::Mouse(){
	mTouchManagerPntr = NULL;
	mTouchId = -1;
}

void Mouse::connect(){
	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	mMouseBeganCallBack = window->getSignalMouseDown().connect(std::bind(&Mouse::mouseBegan, this, std::placeholders::_1));
	mMouseMovedCallBack = window->getSignalMouseDrag().connect(std::bind(&Mouse::mouseMoved, this, std::placeholders::_1));
	mMouseEndCallBack = window->getSignalMouseUp().connect(std::bind(&Mouse::mouseEnded, this, std::placeholders::_1));

	//shared pointer to the Touch Manager
	mTouchManagerPntr = TouchManager::getInstance();
}

Mouse::~Mouse(){
	//Disconnect from the mouse signals
	mMouseBeganCallBack	.disconnect();
	mMouseMovedCallBack	.disconnect();
	mMouseEndCallBack	.disconnect();
	
	//Release the callback objects
	mMouseBeganCallBack	.release();
	mMouseMovedCallBack	.release();
	mMouseEndCallBack	.release();

	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Mouse::disconnect(){
	//Disconnect from the mouse signals
	mMouseBeganCallBack	.disconnect();
	mMouseMovedCallBack	.disconnect();
	mMouseEndCallBack	.disconnect();

	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Mouse::mouseBegan( const cinder::app::MouseEvent &event){
	if (mTouchManagerPntr){
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::BEGAN);

	}
}

void Mouse::mouseMoved( const cinder::app::MouseEvent &event){
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::MOVED);
}

void Mouse::mouseEnded(const  cinder::app::MouseEvent &event){
	if(mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::ENDED);
}