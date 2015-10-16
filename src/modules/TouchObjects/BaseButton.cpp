#include "BaseButton.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace touchObject {

	BaseButton::BaseButton() :BaseTouchObject(),
	mTouchCanceled(false)
	{
	}

	BaseButton::~BaseButton(){
	}

	ButtonRef BaseButton::create(const cinder::vec2 &pos, cinder::vec2 size, std::function <void(touchObject::TouchObjectRef)>callBackFn){
		ButtonRef btnRef(new BaseButton());
		btnRef->setup(true, pos, size);
		btnRef->setCallBackFn(callBackFn);
		return btnRef;
	}

	ButtonRef BaseButton::create(const cinder::vec2 &pos, float radius, std::function <void(touchObject::TouchObjectRef)>callBackFn){
		ButtonRef btnRef(new BaseButton());
		btnRef->setup(true, pos, radius);
		btnRef->setCallBackFn(callBackFn);
		return btnRef;
	}

	ButtonRef BaseButton::create(const std::vector<cinder::vec2> &coordinates, std::function <void(touchObject::TouchObjectRef)>callBackFn){
		ButtonRef btnRef(new BaseButton());
		btnRef->setup(true, coordinates);
		btnRef->setCallBackFn(callBackFn);
		return btnRef;
	}

	void BaseButton::touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType){
		//keep track of the touch position so we can later test how far it has moved
		//This object only takes one touch
		if (mObjectTouchIDs.empty()){
			mObjectTouchIDs.push_back(touchID);
		}
	}

	void BaseButton::touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType){
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

	void BaseButton::touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType){
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
					mOnSelectSignal.emit(shared_from_this());
				}
				catch (...){
					console() << "Missing Function to call" << endl;
				}
			}

			mTouchCanceled = false;
		}
	}

	void BaseButton::draw(){
		gl::color(getObjectColor());
		drawDebugShape();
	}
}