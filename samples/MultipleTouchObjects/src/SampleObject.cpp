#include "SampleObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

SampleObject::SampleObject() : BaseTouchObject(),
mCurPos(vec2(0)),
mPrevPos(vec2(0)),
mTranslationPosition(vec2(20.0f, 30.0f))
{
}

void SampleObject::touchesBeganHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesBeganHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//set the curPos and prevPos to where the mouse just went down
	mCurPos = mPrevPos = touchPnt;
}

void SampleObject::touchesMovedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesMovedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//update positions
	mPrevPos = mCurPos;
	mCurPos = touchPnt;

	//determine the difference between where the mouse is now, compared to where it just came from
	vec2 diff = mCurPos - mPrevPos;
	//update the position
	setPosition(getPosition() + diff);
}

void SampleObject::touchesEndedHandler(int touchID, const cinder::vec2 &touchPnt, touchObject::TouchType touchType){
	//console() << "touchesEndedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
}

//void SampleObject::draw( cinder::vec2 parentTranslatePos) {
void SampleObject::draw(cinder::vec2 parentTranslatePos) {

	console() << "parentTranslatePos  " << mPosition << endl;

	//////////////////////////////
	//OPTION 1 : TO MOVE THE OBJECTS TOGETHER
	//////////////////////////////

	// If the parent of this object translated the space, we need to know about it
	//so we can set the new translation position in the TouchManager. 
	if (parentTranslatePos != vec2(0))
		setTranslationPosition(parentTranslatePos);

	//push & translate
	gl::pushMatrices(); {
		
		gl::translate(mPosition);

		//draw the object
		gl::color(getObjectColor());
		gl::drawSolidCircle(vec2(0), getWidth() / 2);

		
		for (auto child : mChildVector){
			child->draw(mPosition + parentTranslatePos);

			//lines connecting to child
			gl::color(1.0f, 1.0f, 1.0f);
			gl::drawLine(mPosition, child->getPosition());
		}
		
	}gl::popMatrices();

	//for (auto child : mChildVector){
	//	drawDebugShape();
	//}

	//keep the drawing of the debug shape outside the push/pop to see the space that is touchable
	//this is the same way the math works to find the touchable area
	drawDebugShape();




	//////////////////////////////
	//OPTION 2 : TO MOVE THE OBJECTS SEPARATELY
	//////////////////////////////
	/*
	//draw the object
	gl::color(getObjectColor());
	gl::drawSolidCircle(getPosition(), getWidth()/2 );

	for (auto child : mChildVector){
		child->draw();

		drawDebugShape();

		//lines connecting to child
		gl::color(1.0f, 1.0f, 1.0f);
		gl::drawLine(mPosition, child->getPosition());
	}

	drawDebugShape();
	*/
}