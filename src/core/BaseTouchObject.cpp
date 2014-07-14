/*
BaseTouchObject.cpp
Bluecadet, Paul Rudolph
*/

#include "cinder/gl/gl.h"
#include "TouchManager.h"
#include "BaseTouchObject.h"

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
	mPosition(Vec2f::zero()),
	mParentTranslatePosition(Vec2f::zero()),
	mWidth(0.0f),
	mHeight(0.0f),
	mObjectColor(ColorA::white()),
	mTouchesCallbackId(-1),
	mAcceptTouch(true),
	mUniqueID(ObjectID)
{
	TotalObjectCount++;
	ObjectID++;			
	
	
}

BaseTouchObject::~BaseTouchObject(){
	TotalObjectCount--;

	// remove from lookup table
	if (!UniqueIDLookupMap.empty() )
		UniqueIDLookupMap.erase(mUniqueID); 
}

void BaseTouchObject::setup(const cinder::Vec2f &pos, const cinder::Vec2f &size){

	//Store Object in lookup table
	UniqueIDLookupMap[mUniqueID] = TouchObjectWeakRef(shared_from_this());

	setPosition(pos);
	setSize(size);
	registerWithTouchMngr();
}


void  BaseTouchObject::drawDebugBox( bool translating ){
	gl::lineWidth(1.0f);
	
	if (translating)	gl::drawStrokedRect(Rectf(0.0, 0.0, mWidth, mHeight));
	else				gl::drawStrokedRect(getRect());
}

void BaseTouchObject::setPosition( const cinder::Vec2f &pt ){
	mPosition.x =pt.x;
	mPosition.y =pt.y;
}

void BaseTouchObject::registerWithTouchMngr(){
	TouchManager::getInstance()->registerObjectForInput(shared_from_this());   
}

void BaseTouchObject::unRegisterWithTouchMngr(){
	TouchManager::getInstance()->unregisterObjectForInput(shared_from_this());	
}

void BaseTouchObject::endTouches(){
	std::shared_ptr<TouchManager> touchInstance = TouchManager::getInstance();

	for(auto i :mObjectTouchIDs ){
		touchInstance->endTouch(i);
	}
}

bool BaseTouchObject::hasTouchPoint( const Vec2f &pnt ){
	if( getRect().contains(pnt)){
		return true;//point is inside the bounding box 
    }else return false;//point is outside bounding box;
}


//ToString
std::string BaseTouchObject::getDebugString(){
	return "BaseTouchObject: UID: "+mUniqueID;
}


//This Will be overridden for each subclass object
void BaseTouchObject::draw(){	
	//SHOW YOUR TOUCHABLE AREA
	drawDebugBox();
}

};
