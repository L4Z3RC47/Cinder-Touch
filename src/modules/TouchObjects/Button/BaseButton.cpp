/*
BaseButton.cpp
Bluecadet, Paul Rudolph
*/

#include "BaseButton.h"

using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {
	BaseButton::BaseButton() :BaseTouchObject(),
		mTouchCanceled(false)
		
	{
		mToString = "Base Button";
	}

	BaseButton::~BaseButton(){
	}

	ButtonRef BaseButton::create(cinder::Vec2f pos, cinder::Vec2f size, std::function <void(touchObject::TouchObjectRef)>callBackFn){
		ButtonRef btnRef(new BaseButton());
		
		btnRef->registerWithTouchMngr();
		btnRef->setPosition(pos);
		btnRef->setSize(size);
		btnRef->setCallBackFn(callBackFn);
		return btnRef;
	}

	void BaseButton::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		//keep track of the touch position so we can later test how far it has moved
		//This object only takes one touch
		if (mObjectTouchIDs.empty()){
			mObjectTouchIDs.push_back(touchID);
		}
	}

	void BaseButton::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){

		//if the touch point is out of the space of the button = touch canceled.
		if (!mObjectTouchIDs.empty()){
			int currentTouchId = mObjectTouchIDs.front();
			if ((currentTouchId == touchID) && (!hasTouchPoint(touchPnt))){

				//went out of bounds, to the touch was canceled
				mTouchCanceled = true;

				//the button no longer has this touch
				mObjectTouchIDs.clear();
			
			}
		}
	}

	void BaseButton::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){

		if (!mObjectTouchIDs.empty()){
			int currentTouchId = mObjectTouchIDs.front();
			if (currentTouchId == touchID){
				//clear the touch ids
				mObjectTouchIDs.clear();
			}

			//if the touch wasn't canceled already by going out of bounds, then trigger the function call
			if (!mTouchCanceled){
				//Fire off the selected signal here
				try{
						handleTap();
				}
				catch (...){
					console() << "Missing Function to call" << endl;
				}
			}

			mTouchCanceled = false;
		}
	}
	
	void	BaseButton::handleTap(){
		mCallbackFunction(shared_from_this());

	}
	void	BaseButton::draw(cinder::Vec2f translationOffset ){

		gl::pushMatrices(); {
			gl::translate(translationOffset);
		

		
			drawDebugBox();
			//gl::drawStrokedRect(getRect(LOCAL));
			if (!mObjectTouchIDs.empty()){
				gl::drawSolidRect(getRect(LOCAL));
			}

		}gl::popMatrices();
	}
}