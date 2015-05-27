#include "Mouse.h"

using namespace ci::app;

Mouse::Mouse() :
	mTouchManagerPntr(NULL),
	mDebugMode(false),
	mCrtlPressed(false),
	mIsAcceptingCntrPoint(false),
	mDebugCenterPoint(ci::Vec2f::zero()),
	mTouchId( -1)
{

}

void Mouse::connect(bool debugMode){
	//Connect to the application window touch event signals
	ci::app::WindowRef window = cinder::app::getWindow();

	mMouseBeganCallBack	= window->getSignalMouseDown().connect( std::bind( &Mouse::mouseBegan, this, std::placeholders::_1 ));
    mMouseMovedCallBack	= window->getSignalMouseDrag().connect( std::bind( &Mouse::mouseMoved, this, std::placeholders::_1 ));
    mMouseEndCallBack	= window->getSignalMouseUp()  .connect( std::bind( &Mouse::mouseEnded, this, std::placeholders::_1 ));
	
	if (debugMode){
		mDebugMode = debugMode;
		mKeyboardKeyDownCallBack = window->getSignalKeyDown().connect(std::bind(&Mouse::keyDown, this, std::placeholders::_1));
		mKeyboardKeyUpCallBack   = window->getSignalKeyUp()  .connect(std::bind(&Mouse::keyUp,   this, std::placeholders::_1));
	}
	
	//shared pointer to the Touch Manager
	mTouchManagerPntr = TouchManager::getInstance();
}

Mouse::~Mouse(){
	//Disconnect from the mouse signals
	mMouseBeganCallBack	.disconnect();
	mMouseMovedCallBack	.disconnect();
	mMouseEndCallBack	.disconnect();
	if (mDebugMode){
		mKeyboardKeyDownCallBack.disconnect();
		mKeyboardKeyUpCallBack.disconnect();
	}

	//Release the callback objects
	mMouseBeganCallBack	.release();
	mMouseMovedCallBack	.release();
	mMouseEndCallBack	.release();
	if (mDebugMode){
		mKeyboardKeyDownCallBack.release();
		mKeyboardKeyUpCallBack.release();
	}
	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Mouse::disconnect(){
	//Disconnect from the mouse signals
	mMouseBeganCallBack	.disconnect();
	mMouseMovedCallBack	.disconnect();
	mMouseEndCallBack	.disconnect();

	if (mDebugMode){
		mKeyboardKeyDownCallBack.disconnect();
		mKeyboardKeyUpCallBack.disconnect();
	}
	// Remove the pointer to the touch manager
	mTouchManagerPntr = NULL;
}

void Mouse::mouseBegan(const cinder::app::MouseEvent &event){
	if (mTouchManagerPntr){
		if (mCrtlPressed){//IF control is press create the second point
			if (mIsAcceptingCntrPoint){
				mDebugCenterPoint = event.getPos();
				mIsAcceptingCntrPoint = false;
			}else{
				ci::Vec2f oppositePoint = ci::Vec2f((mDebugCenterPoint.x - (event.getPos().x - mDebugCenterPoint.x )),
												    (mDebugCenterPoint.y - (event.getPos().y - mDebugCenterPoint.y)));

				mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::BEGAN);
				mTouchManagerPntr->touchEvent(-2,		 oppositePoint, touchObject::TouchType::MOUSE, TouchManager::BEGAN);
			}
		}else{//Just handle normal Mouse click
			mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::BEGAN);

		}
	}
	

}

void Mouse::mouseMoved( const cinder::app::MouseEvent &event){
	if (mTouchManagerPntr){
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::MOVED);
		
		if (mCrtlPressed){ //update the opposite point
			ci::Vec2f oppositePoint = ci::Vec2f((mDebugCenterPoint.x - (event.getPos().x - mDebugCenterPoint.x)),
												(mDebugCenterPoint.y - (event.getPos().y - mDebugCenterPoint.y)));
			mTouchManagerPntr->touchEvent(-2, oppositePoint, touchObject::TouchType::MOUSE, TouchManager::MOVED);

		}

	}
}

void Mouse::mouseEnded(const  cinder::app::MouseEvent &event){
	if (mTouchManagerPntr){
		mTouchManagerPntr->touchEvent(mTouchId, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::ENDED);
		if (mCrtlPressed){
			mTouchManagerPntr->touchEvent(-2, event.getPos(), touchObject::TouchType::MOUSE, TouchManager::ENDED);
		}
	
	}
}

void Mouse::keyDown(const cinder::app::KeyEvent &event){
	//Check if Cntrol is pressed for the first time
	if (!mCrtlPressed && event.isControlDown()){
		mCrtlPressed = true;
		mIsAcceptingCntrPoint = true;
	}
}

void Mouse::keyUp(const cinder::app::KeyEvent &event){
	//Check if Cntrol is released 
	if (mCrtlPressed && !event.isControlDown()){
		mCrtlPressed = false;
		mIsAcceptingCntrPoint = false;
		mDebugCenterPoint = ci::Vec2f::zero();
		mTouchManagerPntr->touchEvent(-2, ci:: Vec2f::zero(), touchObject::TouchType::MOUSE, TouchManager::ENDED);
	}
}