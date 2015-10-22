#include "cinder/gl/gl.h"
#include "TouchManager.h"
#include "BaseTouchObject.h"
#include "boost/lexical_cast.hpp"
#include <math.h>

using namespace std;
using namespace ci;
using namespace ci::app;

namespace touchObject {

/*	
Total object count is a value that is incremented when an gui object is created and decremented when a gui object is destroyed that way we will allways know how many gui objects exist at any moment.
the ObjectID is only incremented when a new object is created, so any time a new object is created it gets its own unique number. 
*/

// This keeps track of how many gui objects are currently alive, this number is incremented and decremented 
	int	BaseTouchObject::TotalObjectCount = 0;
// This number will only be incremented when a gui object is created, thus making it unique.
	int	BaseTouchObject::ObjectID = 0;
	//Declare Static lookup map
	TouchObjectMap BaseTouchObject::UniqueIDLookupMap;

	BaseTouchObject::BaseTouchObject() :
		mTranslationPos(vec2(0)),
		mWidth(0.0f),
		mHeight(0.0f),
		mObjectColor(ColorA::white()),
		mTouchesCallbackId(-1),
		mAcceptTouch(true),
		mUniqueID(ObjectID),
		mScale(vec2(1.0f)),
		mPosition(vec2(0))
	{
		TotalObjectCount++;
		ObjectID++;
	}

	BaseTouchObject::~BaseTouchObject(){
		TotalObjectCount--;

		// remove from lookup table
		if (!UniqueIDLookupMap.empty())
			UniqueIDLookupMap.erase(mUniqueID);

		mPath.clear();
	}

	void BaseTouchObject::setupBaseTouchObj(const cinder::vec2 &pos, const cinder::vec2 &size, bool registerWithTouchManager){
		//Store Object in lookup table
		UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());
		//register with the touch manager
		if (registerWithTouchManager) registerWithTouchMngr();
		//set position
		mPosition = pos;

		//create shape from this position and size
		vector<cinder::vec2> coordinates;
		coordinates.push_back(pos);
		coordinates.push_back(vec2(pos.x + size.x, pos.y));
		coordinates.push_back(vec2(pos.x + size.x, pos.y + size.y));
		coordinates.push_back(vec2(pos.x, pos.y + size.y));
		createShape(coordinates);
		//clean up
		coordinates.clear();
	}

	void BaseTouchObject::setupBaseTouchObj(const cinder::vec2 &pos, float radius, bool registerWithTouchManager){
		//Store Object in lookup table
		UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());
		//register with the touch manager
		if (registerWithTouchManager) registerWithTouchMngr();
		//set position
		mPosition = pos;

		int points = 100;
		double slice = 2 * M_PI / points;
		vector<cinder::vec2> coordinates;
		for (int i = 0; i < points; i++){
			double angle = slice * i;
			int newX = (int)(radius *  cos(angle));
			int newY = (int)(radius * sin(angle));
			coordinates.push_back(vec2(newX, newY));
		}
		createShape(coordinates);
		//clean up
		coordinates.clear();
	}

	void BaseTouchObject::setupBaseTouchObj(const std::vector<cinder::vec2> &coordinates, const cinder::vec2 &pos, bool registerWithTouchManager){
		//Store Object in lookup table
		UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());
		//register with the touch manager
		if (registerWithTouchManager) registerWithTouchMngr();
		//
		createShape(coordinates);
	}

	void BaseTouchObject::createShape(const std::vector<cinder::vec2> &coordinates){
		//iterate through the coordinates passed in, creating a path where the touchable area is
		for (int i = 0; i <= coordinates.size() - 1; i++){
			if (i == 0){						//move to the first coordinate
				mPath.moveTo(coordinates[0]);
			}
			else{								//add all the segments of the shape
				mPath.lineTo(coordinates[i]);
			}
		}
	}

	void BaseTouchObject::setScale(const cinder::vec2 &scale){
		if (mScale != scale){
			mScale = scale;

			//for now everything will be scaled around the center -- can build this out later
			vec2 scaleCenter = mPath.calcPreciseBoundingBox().getCenter();
			mPath.scale(scale, scaleCenter);
		}
	}

	void BaseTouchObject::registerWithTouchMngr(){
		TouchManager::getInstance()->registerObjectForInput(shared_from_this());
	}

	void BaseTouchObject::unRegisterWithTouchMngr(){
		TouchManager::getInstance()->unregisterObjectForInput(shared_from_this());
	}

	void BaseTouchObject::endTouches(){
		std::shared_ptr<TouchManager> touchInstance = TouchManager::getInstance();

		for (auto i : mObjectTouchIDs){
			touchInstance->endTouch(i);
		}
	}

	void BaseTouchObject::drawDebugShape(){
		gl::color(getObjectColor());
		gl::lineWidth(2.0f);

		//NOT TRANSLATING
		if (mTranslationPos == vec2(0)){
			gl::pushModelView(); {
				gl::translate(mPosition);
				gl::draw(mPath, mScale.x);
			}gl::popModelView();
		}
		else{
			gl::translate(mPosition);
			gl::draw(mPath, mScale.x);
		}
	}

	bool BaseTouchObject::hasTouchPoint(const vec2 &pnt){
		//NOT TRANSLATING
		if (mTranslationPos == vec2(0)){
			//checking the global point minus the position will give us the shape coordintes at (0,0) to check against
			if (mPath.contains(pnt - mPosition)) return true;
			else								 return false;
		}
		else{
			if (mPath.contains(pnt - mTranslationPos - mPosition)) return true;
			else														 return false;
		}
	}

	//ToString
	std::string BaseTouchObject::getDebugString(){
		return "BaseTouchObject: UID: " + boost::lexical_cast<string>(mUniqueID);
	}

	//This Will be overridden for each subclass object
	void BaseTouchObject::draw(){
		//SHOW YOUR TOUCHABLE AREA
		drawDebugShape();
	}
};