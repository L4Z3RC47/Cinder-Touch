#include "SampleObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

SampleObject::SampleObject() : BaseTouchObject(),
mCurPos(vec2(0)),
mPrevPos(vec2(0)),
mParentTranslatePos(vec2(0))
{
}

void SampleObject::setup(const vec2 &position, const vec2 &size, const ColorA &color) {
	////////////////
	//TOUCH SETTINGS
	////////////////
	//set default position
	setPosition(position);	
	//MUST set a size in order for the touch to find the object
	setSize(size);			
	//you are not required to set an object color. The default is white. 
	setObjectColor(color);
	//register 
	registerWithTouchMngr();
	//tell the TouchManager whether or not to funnel touches to this object, or to skip this object and keep looking below
	setAcceptTouch(true);
}

void SampleObject::touchesBeganHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	console() << "touchesBeganHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//set the curPos and prevPos to where the mouse just went down
	mCurPos = mPrevPos = touchPnt;
}

void SampleObject::touchesMovedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	console() << "touchesMovedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//update positions
	mPrevPos = mCurPos;
	mCurPos = touchPnt;

	//determine the difference between where the mouse is now, compared to where it just came from
	vec2 diff = mCurPos - mPrevPos;
	//update the position
	setPosition(getPosition() + diff);
}

void SampleObject::touchesEndedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	console() << "touchesEndedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
}

void SampleObject::draw( cinder::vec2 parentTranslatePos) {
	
		
	//If the parent of this object translated the space, we need to know about it 
	//so we can set the new translation position in the TouchManager. 
	if (parentTranslatePos != vec2(0))
		setParentTranslatePosition(parentTranslatePos);

	//////////////////////////////
	//OPTION 1 : TO MOVE THE OBJECTS TOGETHER
	//////////////////////////////

	//push & translate
	gl::pushMatrices(); {
		gl::translate(mPosition);
		setTranslating(true);

		//draw the object
		gl::color(getObjectColor());
		gl::drawSolidCircle(getSize() / 2.0f, getWidth() / 2.0f); //center the circle in the middle of the touch area
		
		//SHOW YOUR TOUCHABLE AREA :: this is a debug option
		drawDebugBox(true);

		//draw the children of this object
		for (auto child : mChildVector){
			child->draw(mPosition + parentTranslatePos);

			//line connecting to child
			gl::color(1.0f, 1.0f, 1.0f);
			gl::drawLine(vec2(0) + getSize() / 2.0f, child->getPosition() + child->getSize() / 2.0f);
		}
	}gl::popMatrices();




	//////////////////////////////
	//OPTION 2 : TO MOVE THE OBJECTS SEPARATELY
	//////////////////////////////

	/*
	//draw the object
	gl::color(getObjectColor());
	gl::drawSolidCircle(mPosition + getSize() / 2.0f, getWidth() / 2.0f); //center the circle in the middle of the touch area

	for (auto child : mChildVector){
		child->draw();

		drawDebugBox(false);

		//lines connecting to child
		gl::color(1.0f, 1.0f, 1.0f);
		gl::drawLine(mPosition + getSize() / 2.0f, child->getPosition() + child->getSize() / 2.0f);
	}
	*/

}