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
		mTranslationPosition(vec2(0)),
		mWidth(0.0f),
		mHeight(0.0f),
		mObjectColor(ColorA::white()),
		mTouchesCallbackId(-1),
		mAcceptTouch(true),
		mUniqueID(ObjectID),
		mScale(vec2(1.0f))
	{
		TotalObjectCount++;
		ObjectID++;
	}

	BaseTouchObject::~BaseTouchObject(){
		TotalObjectCount--;

		// remove from lookup table
		if (!UniqueIDLookupMap.empty())
			UniqueIDLookupMap.erase(mUniqueID);

		mShape2d.clear();
	}

	//this setup function is going to take your position, & size, and create a shape from that
	void BaseTouchObject::setup(bool registerWithTouchManager, const cinder::vec2 &pos, const cinder::vec2 &size){

		//Store Object in lookup table
		UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());

		if (registerWithTouchManager) registerWithTouchMngr();

		setSize(size);

		//create shape from this position and size
		vector<cinder::vec2> coordinates;
		coordinates.push_back(pos);
		coordinates.push_back(vec2(pos.x + size.x, pos.y));
		coordinates.push_back(vec2(pos.x + size.x, pos.y + size.y));
		coordinates.push_back(vec2(pos.x, pos.y + size.y));

		createShape2d(coordinates);
		coordinates.clear();
	}

	void BaseTouchObject::setup(bool registerWithTouchManager, const std::vector<cinder::vec2> &coordinates){

		//Store Object in lookup table
		UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());

		if (registerWithTouchManager) registerWithTouchMngr();

		createShape2d(coordinates);
	}

	void BaseTouchObject::setup(bool registerWithTouchManager, const cinder::vec2 &pos, float radius){

		//Store Object in lookup table
		UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());

		if (registerWithTouchManager) registerWithTouchMngr();


		int points = 100;
		double slice = 2 * M_PI / points;
		vector<cinder::vec2> coordinates;

		for (int i = 0; i < points; i++){
			double angle = slice * i;
			int newX = (int)(pos.x + radius *  cos(angle));
			int newY = (int)(pos.y + radius * sin(angle));
			coordinates.push_back(vec2(newX, newY));
		}
		
		createShape2d(coordinates);
		coordinates.clear();

	}

	void BaseTouchObject::createShape2d(const std::vector<cinder::vec2> &coordinates){
		for (int i = 0; i <= coordinates.size() - 1; i++){
			if (i == 0){						//move to the first coordinate
				mShape2d.moveTo(coordinates[0]);
			}
			else{								//add all the segments of the shape
				mShape2d.lineTo(coordinates[i]);
			}
		}
	}

	void BaseTouchObject::setScale(const cinder::vec2 &scale){
		if (mScale != scale){
			mScale = scale;

			//for now everything will be scaled around the center -- can build this out later
			vec2 scaleCenter = mShape2d.calcPreciseBoundingBox().getCenter();
			mShape2d.scale(scale, scaleCenter);
		}
	}

	void BaseTouchObject::drawDebugShape(){
		gl::lineWidth(2.0f);
		gl::drawSolid(mShape2d);
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

	bool BaseTouchObject::hasTouchPoint(const vec2 &pnt){
		if (mShape2d.contains(pnt - mTranslationPosition))
			return true;
		else
			return false;
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