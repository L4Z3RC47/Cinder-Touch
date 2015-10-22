#include "BlueObj.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

BlueObj::BlueObj() : BaseTouchObject(),
mRadius(50.0f),
mCurPos(vec2(0)),
mPrevPos(vec2(0))
//mParentTranslatePos(vec2(0))
{
}

void BlueObj::setup(){
	//required
	setupBaseTouchObj(mCurPos, mRadius);
	setObjectColor(ColorA(1.0f, 0.0f, 0.0f, 1.0f));
}

void BlueObj::touchesBeganHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesBeganHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
	//set the curPos and prevPos to where the mouse just went down
	mCurPos = mPrevPos = touchPnt;
}

void BlueObj::touchesMovedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesMovedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//update positions
	mPrevPos = mCurPos;
	mCurPos = touchPnt;

	//determine the difference between where the mouse is now, compared to where it just came from
	vec2 diff = mCurPos - mPrevPos;
	//update the position
	setPosition(getPosition() + diff);
}

void BlueObj::touchesEndedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesEndedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
}

void BlueObj::draw(cinder::vec2 parentTranslatePos) {

	//console() << "parentTranslatePos = " << parentTranslatePos << endl;

	//need to set it 
	setTranslationPos(parentTranslatePos);

	//draw what you want on this touchable object
	gl::color(ColorA(0.0f, 0.0f, 1.0f, 0.75f));
	gl::drawSolidCircle(getPosition(), mRadius);

	//draw the touch shape above
	drawDebugShape();
}