#include "Mouse.h"

using namespace ci::app;

Mouse::Mouse(){
	mTouchManagerPntr = NULL;
	mTouchId = -1;
}

Mouse::~Mouse(){
	//Disconnect from the mouse signals
	mMouseBeganConnection.disconnect();
	mMouseMovedConnection.disconnect();
	mMouseEndConnection.disconnect();

	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Mouse::connect(){
	//console() << "Mouse connected" <<std::endl;
	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	mMouseBeganConnection	= window->getSignalMouseDown().connect(std::bind(&Mouse::mouseBegan, this, std::placeholders::_1));
	mMouseMovedConnection	= window->getSignalMouseDrag().connect(std::bind(&Mouse::mouseMoved, this, std::placeholders::_1));
	mMouseEndConnection		= window->getSignalMouseUp().connect(std::bind(&Mouse::mouseEnded, this, std::placeholders::_1));

	//shared pointer to the Touch Manager
	mTouchManagerPntr = TouchManager::getInstance();
}

void Mouse::disconnect(){
	//Disconnect from the mouse signals
	mMouseBeganConnection.disconnect();
	mMouseMovedConnection.disconnect();
	mMouseEndConnection.disconnect();

	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Mouse::mouseBegan( const cinder::app::MouseEvent &event){
	//console() << "mouse began" << std::endl;
	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::BEGAN);
}

void Mouse::mouseMoved( const cinder::app::MouseEvent &event){
	//console() << "mouseMoved" << std::endl;

	if (mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::MOVED);
}

void Mouse::mouseEnded(const  cinder::app::MouseEvent &event){
	//console() << "mouseEnded" << std::endl;

	if(mTouchManagerPntr)
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::ENDED);
}