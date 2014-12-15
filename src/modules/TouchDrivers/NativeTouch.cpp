#include "NativeTouch.h"

using namespace ci::app;

NativeTouch::NativeTouch(){
	mTouchManagerPntr = NULL;
	mTouchId = -1;
}

void NativeTouch::connect(){
	//console() << "SystemDriver::connect" << std::endl;

	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	if (window){
		//began
		mTouchBeganCallBack = window->getSignalTouchesBegan().connect(std::bind(&NativeTouch::nativeTouchBegan, this, std::placeholders::_1));
		mTouchMovedCallBack = window->getSignalTouchesMoved().connect(std::bind(&NativeTouch::nativeTouchMoved, this, std::placeholders::_1));
		mTouchEndCallBack = window->getSignalTouchesEnded().connect(std::bind(&NativeTouch::nativeTouchEnded, this, std::placeholders::_1));
	}
	
	//shared pointer to the Touch Manager
	mTouchManagerPntr = TouchManager::getInstance();
}

NativeTouch::~NativeTouch(){
	//Disconnect from the mouse signals
	mTouchBeganCallBack	.disconnect();
	mTouchMovedCallBack	.disconnect();
	mTouchEndCallBack	.disconnect();
	
	//Release the callback objects
	mTouchBeganCallBack	.release();
	mTouchMovedCallBack	.release();
	mTouchEndCallBack	.release();

	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void NativeTouch::disconnect(){
	//Disconnect from the mouse signals
	mTouchBeganCallBack	.disconnect();
	mTouchMovedCallBack	.disconnect();
	mTouchEndCallBack	.disconnect();

	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void NativeTouch::nativeTouchBegan(const cinder::app::TouchEvent &event){
//	console() << "------------NativeTouch::nativeTouchBegan" << std::endl;
	if (mTouchManagerPntr){
		for (TouchEvent::Touch touch : event.getTouches())
			mTouchManagerPntr->touchEvent(touch.getId(), touch.getPos(), touchObject::TouchType::TOUCH, TouchManager::BEGAN);
	}
}

void NativeTouch::nativeTouchMoved(const cinder::app::TouchEvent &event){
//	console() << "------------NativeTouch::nativeTouchMoved" << std::endl;
	if (mTouchManagerPntr){
		for (TouchEvent::Touch touch : event.getTouches())
			mTouchManagerPntr->touchEvent(touch.getId(), touch.getPos(), touchObject::TouchType::TOUCH, TouchManager::MOVED);
	}
}

void NativeTouch::nativeTouchEnded(const  cinder::app::TouchEvent &event){
//	console() << "------------NativeTouch::nativeTouchEnded" << std::endl;
	if (mTouchManagerPntr){
		for (TouchEvent::Touch touch : event.getTouches())
			mTouchManagerPntr->touchEvent(touch.getId(), touch.getPos(), touchObject::TouchType::TOUCH, TouchManager::ENDED);
	}
}