#include "TouchManager.h"
#include "boost/lexical_cast.hpp"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;

std::shared_ptr<TouchManager> TouchManager::mTouchManagerInstance;

TouchManager::TouchManager():mScale(ci::vec2(1,1)){
}

TouchManager::~TouchManager(){
	//console()<<"TouchManager destructor"<<endl;
	mRegisteredObjectsDeque.clear();
	mTouchMap.clear();
}

std::shared_ptr<TouchManager> TouchManager::getInstance(){
	//If the manager is not set up yet, create it and return a pointer to the caller
	if (!mTouchManagerInstance)
		mTouchManagerInstance = std::shared_ptr<TouchManager>(new TouchManager());
	return mTouchManagerInstance;
}

void TouchManager::update(){
	updateTouches();
}

void TouchManager::updateTouches(){
	//console() << "update touches" << endl;
	mUpdateMutex.lock();
		if (!mTouchUpdateQueue.empty()){
			for (std::pair<TouchEventType, TouchObject> touchEvent : mTouchUpdateQueue){
				TouchEventType touchEventType =touchEvent.first;
				TouchObject	   touchObject = touchEvent.second;
				switch (touchEventType){
					case BEGAN:mainThreadTouchesBegan(touchObject.touchId, touchObject.touchPoint, touchObject.touchType); break;
					case MOVED:mainThreadTouchesMoved(touchObject.touchId, touchObject.touchPoint, touchObject.touchType); break;
					case ENDED:mainThreadTouchesEnded(touchObject.touchId, touchObject.touchPoint, touchObject.touchType); break;
				}
			}
			mTouchUpdateQueue.clear();
		}
	mUpdateMutex.unlock();
}
void TouchManager::touchEvent(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType, TouchEventType eventType){
	//console() << "TouchManager::touchEvent" << endl;

    //update the most recent touch time on the app
    mLatestTouchTime = getElapsedSeconds();
    
	TouchObject currentTouchObj;
	currentTouchObj.touchId = touchID;
	currentTouchObj.touchPoint = touchPnt/ mScale;
	currentTouchObj.touchType = touchType;

	std::pair<TouchEventType, TouchObject> currentTouchEvent = std::pair<TouchEventType, TouchObject>(eventType, currentTouchObj);
	mUpdateMutex.lock();
		mTouchUpdateQueue.push_back(currentTouchEvent);
	mUpdateMutex.unlock();
}

void TouchManager::mainThreadTouchesBegan(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType){

		mTouchMapLock.lock();

			//specifically for the markers if you move them quickly -- but don't want to add a new object if the old one still exists
			touchObject::TouchObjectRef foundObject = findTouchingObject(touchPnt);

			if (foundObject){
				//Initialize a new touch object 
				TouchObject touchObject;
				touchObject.touchId = touchID;
				touchObject.touchPoint = touchPnt;
				touchObject.touchType = touchType;
				touchObject.touchingObjectPntr = foundObject;

				mTouchMap[touchID] = touchObject;

				// call the touch began on the found object
				foundObject->touchesBeganHandler(touchID, touchPnt, touchType);
			}
		mTouchMapLock.unlock();
	
}

void TouchManager::mainThreadTouchesMoved(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType){

	//if the touch has been reassigned or just skipped the began, manually make it go through and add a new touch object
	if (mTouchMap.count(touchID) == 0 && touchType == touchObject::TouchType::OBJECT){
		//Sometimes you get non consistent touch inputs. Rather than always being DOWN, MOVE, UP, your system might give you DOWN, MOVE, DOWN, MOVE, MOVE. 
		//If for some reason the object skipped the "mainThreadTouchesBegan" and is not part 
		//of the touchMap yet, this will force it there first, before allowing the move. 
		mainThreadTouchesBegan(touchID, touchPnt, touchType);
	}else{

		//lock before adjustng the map
		mTouchMapLock.lock();
		//there the touchID exists in the map
		if (mTouchMap.count(touchID) > 0){
			//set the touchpoint to equal the new point
			mTouchMap[touchID].touchPoint = touchPnt;

			//call the move over the object that the touch is on
			if (mTouchMap[touchID].touchingObjectPntr){

				//for normal finger touches
				if (touchType != touchObject::TouchType::OBJECT){
					mTouchMap[touchID].touchingObjectPntr->touchesMovedHandler(touchID, touchPnt, touchType); //NEED TO UPDATE WITH BASE CLASS
				}
				else if (touchType == touchObject::TouchType::OBJECT){

					/////////
					//this was for the screens in JPII -- to change the object the touch was over when it crossed the center line
					//don't want to remove yet but should discuss - related to reassigning the touch
					////////
					touchObject::TouchObjectRef origFoundObject = mTouchMap[touchID].touchingObjectPntr;
					touchObject::TouchObjectRef curFoundObject = findTouchingObject(touchPnt);

					//if it's the same object as it was before, continue as usual
					if (origFoundObject == curFoundObject)
						mTouchMap[touchID].touchingObjectPntr->touchesMovedHandler(touchID, touchPnt, touchType);//NEED TO UPDATE WITH BASE CLASS
					else{
						//							console() << "reassign-------------------" << endl;
						if (mTouchMap[touchID].touchingObjectPntr){
							mTouchMap[touchID].touchingObjectPntr->touchesEndedHandler(touchID, touchPnt, touchType);//NEED TO UPDATE WITH BASE CLASS
						}

						mTouchMap[touchID].touchingObjectPntr = curFoundObject;

						if (mTouchMap[touchID].touchingObjectPntr){
							mTouchMap[touchID].touchingObjectPntr->touchesBeganHandler(touchID, touchPnt, touchType);//NEED TO UPDATE WITH BASE CLASS
						}

					}

				}
			}
		}
		mTouchMapLock.unlock();
	}
}

void TouchManager::mainThreadTouchesEnded(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType){
	mTouchMapLock.lock();
	//if the touch belongs to an object, find that object and call touches ended on it
	if (mTouchMap[touchID].touchingObjectPntr) 
		mTouchMap[touchID].touchingObjectPntr->touchesEndedHandler(touchID, touchPnt, touchType);
	//remove the touchID from the touchMap
	mTouchMap.erase(touchID);
	mTouchMapLock.unlock();
}

void TouchManager::registerObjectForInput(touchObject::TouchObjectRef obj){
	mRegisteredObjectLock.lock();
	mRegisteredObjectsDeque.push_front(obj);
	mRegisteredObjectLock.unlock();
}

void TouchManager::unregisterObjectForInput(touchObject::TouchObjectRef obj){
	//if there are registered objects in the deque
	if (mRegisteredObjectsDeque.size() > 0){
		int i = 0;
		for (auto item : mRegisteredObjectsDeque){
			//find which one it is
			if (item->getUniqueID() == obj->getUniqueID()){
				//lock the deque so no changes can be made while an object is being removed
				mRegisteredObjectLock.lock();
				mRegisteredObjectsDeque.erase(mRegisteredObjectsDeque.begin() + i);
				mRegisteredObjectLock.unlock();
			}
			i++;
		}
	}
}

ci::vec2 TouchManager::getCurrentTouchPoint(int touchId){
	vec2 pnt = vec2(-1.0f, -1.0f);
	std::map<int, TouchObject>::iterator itr = mTouchMap.find(touchId);

	if (itr != mTouchMap.end())
		pnt = mTouchMap[touchId].touchPoint;
	return pnt;
}

void TouchManager::endTouch(int touchID){
	//LOCK
	mTouchMapLock.lock();
	//
	mTouchMap[touchID].touchingObjectPntr = touchObject::TouchObjectRef();
	//UNLOCK
	mTouchMapLock.unlock();
}


touchObject::TouchObjectRef TouchManager::findTouchingObject(const ci::vec2 &touchPoint){
	/*
	we need to decide who gets the touch, and not based on who was registered with the touch manager first.
	That is what the current setup is doing.do we first loop through a
	*/

	//if there are objects in the deque
	if (mRegisteredObjectsDeque.size() > 0){
		for (auto item : mRegisteredObjectsDeque){
			if (item->hasTouchPoint(touchPoint) && item->isAcceptingTouch()){
				return item;
			}
		}
	}

	return NULL;
}

void TouchManager::draw(){
	mTouchMapLock.lock();
		for (auto touch : mTouchMap) {
			//ID of the touch
			int touchId = touch.first;
			//location of the touch
			vec2 point = touch.second.touchPoint;

			gl::color(ColorA(1.0f, 0.0f, 0.0f, 0.5f));
			gl::lineWidth(1.0f);
			gl::drawSolidCircle(point / mScale, 10.0f);

			//draw the label on the touch
			gl::enableAlphaBlending();
			gl::drawString(to_string(touchId), point - vec2(-1.0f, +40.0), cinder::Color(1, 0, 0), Font("arial", 12.0f));
			gl::disableAlphaBlending();
		}
	mTouchMapLock.unlock();
}