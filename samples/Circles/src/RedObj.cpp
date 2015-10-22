#include "RedObj.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

RedObj::RedObj() : BaseTouchObject(),
mRadius(100.0f),
mCurPos(vec2(0)),
mPrevPos(vec2(0))
{
}

void RedObj::setup(){
	//required for touches
	setupBaseTouchObj(mCurPos, mRadius);
	setObjectColor(ColorA(1.0f, 0.0f, 0.0f, 1.0f));
	setScale(vec2(2.0f, 2.0f));

	//add a blue object 
	mBlueObj = std::shared_ptr<class BlueObj>(new BlueObj());
	mBlueObj->setup();
}

void RedObj::touchesBeganHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesBeganHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
	//set the curPos and prevPos to where the mouse just went down
	mCurPos = mPrevPos = touchPnt;
}

void RedObj::touchesMovedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesMovedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//update positions
	mPrevPos = mCurPos;
	mCurPos = touchPnt;

	//determine the difference between where the mouse is now, compared to where it just came from
	vec2 diff = mCurPos - mPrevPos;

	//update the position
	setPosition(getPosition() + diff);
}

void RedObj::touchesEndedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesEndedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
}

void RedObj::draw() {

	gl::color(ColorA(1.0f, 0.0f, 0.0f, 0.5f));

	
	//OPTION A: translating space
	vec2 translationPos = vec2(100.0f, 100.0f);
	
	gl::pushModelView(); {
		gl::translate(translationPos);
		setTranslationPos(translationPos); //on touchable objects, you must tell the object what space you are translating
	
		gl::drawSolidCircle(getPosition(), mRadius * getScale().x);

		//draw the touch shape above
		drawDebugShape();

		//draw sub object. Let it know what the parent is translating + the parents current postion
		mBlueObj->draw(translationPos + getPosition());

	}gl::popModelView();
	//END OPTION A	

	/*
	//OPTION B: not translating space
	gl::drawSolidCircle(getPosition(), mRadius);
	//draw the touch shape above
	drawDebugShape();
	
	//draw sub object.
	mBlueObj->draw();
	//END OPTION B
	*/	
}