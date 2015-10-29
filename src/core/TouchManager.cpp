#include "TouchManager.h"
#include "boost/lexical_cast.hpp"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;

std::shared_ptr<TouchManager> TouchManager::mTouchManagerInstance;

TouchManager::TouchManager():
mScale(ci::vec2(1,1)),
mTotalRegisteredObjectCount(0)
{
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
			touchObject::TouchObjectWeakRef foundObject = findTouchingObject(touchPnt);

			if (auto objPtr = foundObject.lock()){
				//Initialize a new touch object 
				TouchObject touchObject;
				touchObject.touchId = touchID;
				touchObject.touchPoint = touchPnt;
				touchObject.touchType = touchType;
				touchObject.touchingObjectPntr = foundObject;

				mTouchMap[touchID] = touchObject;

				// call the touch began on the found object
				objPtr->touchesBeganHandler(touchID, touchPnt, touchType);
				
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

		auto touchObjPtr = mTouchMap[touchID].touchingObjectPntr.lock();


		//there the touchID exists in the map
		if (mTouchMap.count(touchID) > 0){
			//set the touchpoint to equal the new point
			mTouchMap[touchID].touchPoint = touchPnt;

			//call the move over the object that the touch is on
			if (touchObjPtr){

				//for normal finger touches
				if (touchType != touchObject::TouchType::OBJECT){
					touchObjPtr->touchesMovedHandler(touchID, touchPnt, touchType); //NEED TO UPDATE WITH BASE CLASS
				}
				else if (touchType == touchObject::TouchType::OBJECT){

					/////////
					//this was for the screens in JPII -- to change the object the touch was over when it crossed the center line
					//don't want to remove yet but should discuss - related to reassigning the touch
					////////

					auto origFoundObject = mTouchMap[touchID].touchingObjectPntr.lock();
					auto curFoundObject = findTouchingObject(touchPnt).lock();

					//if it's the same object as it was before, continue as usual
					if (origFoundObject == curFoundObject)
						touchObjPtr->touchesMovedHandler(touchID, touchPnt, touchType);//NEED TO UPDATE WITH BASE CLASS
					else{
						//							console() << "reassign-------------------" << endl;
						if (touchObjPtr){
							touchObjPtr->touchesEndedHandler(touchID, touchPnt, touchType);//NEED TO UPDATE WITH BASE CLASS
						}

						mTouchMap[touchID].touchingObjectPntr = curFoundObject;

						if (touchObjPtr){
							touchObjPtr->touchesBeganHandler(touchID, touchPnt, touchType);//NEED TO UPDATE WITH BASE CLASS
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
	auto touchObjPtr = mTouchMap[touchID].touchingObjectPntr.lock();
	//if the touch belongs to an object, find that object and call touches ended on it
	if (touchObjPtr)
		touchObjPtr->touchesEndedHandler(touchID, touchPnt, touchType);
	//remove the touchID from the touchMap
	mTouchMap.erase(touchID);
	mTouchMapLock.unlock();
}

void TouchManager::registerObjectForInput(touchObject::TouchObjectWeakRef obj){
	mRegisteredObjectLock.lock();
	mRegisteredObjectsDeque.push_front(obj);
	mTotalRegisteredObjectCount++;
	mRegisteredObjectLock.unlock();
}

void TouchManager::unregisterObjectForInput(touchObject::TouchObjectWeakRef obj){
	try{
		mRegisteredObjectLock.lock();
		deque<touchObject::TouchObjectWeakRef>::iterator iter;
		int i = 0;
		auto objPtr = obj.lock();

		for (iter = mRegisteredObjectsDeque.begin(); iter != mRegisteredObjectsDeque.end();){

			auto iterPtr = (*iter).lock();

			if (iterPtr->getUniqueID() == objPtr->getUniqueID()){

				iter = mRegisteredObjectsDeque.erase(mRegisteredObjectsDeque.begin() + i);
				mTotalRegisteredObjectCount--;

				break;
			}
			else{
				iter++;
				i++;
			}
		}
		mRegisteredObjectLock.unlock();
	}
	catch (std::exception e){
		console() << e.what() << endl;
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
	mTouchMap[touchID].touchingObjectPntr = touchObject::TouchObjectWeakRef();
	//UNLOCK
	mTouchMapLock.unlock();
}


touchObject::TouchObjectWeakRef TouchManager::findTouchingObject(const ci::vec2 &touchPoint){
	/*
	we need to decide who gets the touch, and not based on who was registered with the touch manager first.
	That is what the current setup is doing.do we first loop through a
	*/
	try{
		mRegisteredObjectLock.lock();
		if (mRegisteredObjectsDeque.size() > 0){
			for (auto item : mRegisteredObjectsDeque){

				auto itemPtr = item.lock();
				if (itemPtr && itemPtr->hasTouchPoint(touchPoint) && itemPtr->isAcceptingTouch()){
					mRegisteredObjectLock.unlock();
					return item;
				}
			}
		}
		mRegisteredObjectLock.unlock();
	}
	catch(std::exception e){
		console() << e.what() << endl;
	}

	return touchObject::TouchObjectWeakRef();
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