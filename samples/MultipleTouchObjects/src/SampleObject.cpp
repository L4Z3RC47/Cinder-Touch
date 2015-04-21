#include "SampleObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

SampleObject::SampleObject() : BaseTouchObject(),
mCurPos(Vec2f(0.0f, 0.0f)),
mPrevPos(Vec2f(0.0f, 0.0f))
{
}

void SampleObject::setup(const Vec2f &position, const Vec2f &size, const ColorA &color) {
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

void SampleObject::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
	console() << "touchesBeganHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//set the curPos and prevPos to where the mouse just went down
	mCurPos = mPrevPos = touchPnt;
}

void SampleObject::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
	console() << "touchesMovedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;

	//update positions
	mPrevPos = mCurPos;
	mCurPos = touchPnt;

	//determine the difference between where the mouse is now, compared to where it just came from
	Vec2f diff = mCurPos - mPrevPos;
	//update the position
	setPosition(getPosition() + diff);
}

void SampleObject::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
	console() << "touchesEndedHandler :: touchID = " << touchID << " touchPnt = " << touchPnt << " touchType = " << touchType << endl;
}
void	SampleObject::setPosition(const cinder::Vec2f &pt){
	BaseTouchObject::setPosition(pt);

	for (auto child : mChildVector){
		child->setParentPosition(getGlobalPosition());
	}
}
void SampleObject::draw(cinder::Vec2f translationOffset) {
	
		

	//////////////////////////////
	//OPTION 1 : TO MOVE THE OBJECTS TOGETHER
	//////////////////////////////

	//push & translate
	gl::pushMatrices(); {
		gl::translate(translationOffset);
		console() << "TRANSLATION OFFSET " << translationOffset << endl;


		//draw the object
		gl::color(getObjectColor());
		gl::drawSolidCircle(getRect(LOCAL).getCenter(), getWidth()/ 2.0f); //center the circle in the middle of the touch area
		
		//SHOW YOUR TOUCHABLE AREA :: this is a debug option
		drawDebugBox();

		//draw the children of this object
		for (auto child : mChildVector){
			child->draw(getPosition());

			//line connecting to child
			gl::color(1.0f, 1.0f, 1.0f);
			gl::drawLine(getCenter(), child->getCenter() + getPosition() );
		}

	}gl::popMatrices();
	



	//////////////////////////////
	//OPTION 2 : TO MOVE THE OBJECTS SEPARATELY
	//////////////////////////////

/*	
	//draw the object
	gl::color(getObjectColor());
	gl::drawSolidCircle(mPosition + getSize() / 2, getWidth() / 2); //center the circle in the middle of the touch area

	for (auto child : mChildVector){
		child->draw();

		drawDebugBox(false);

		//lines connecting to child
		gl::color(1.0f, 1.0f, 1.0f);
		gl::drawLine(mPosition + getSize() / 2, child->getPosition() + child->getSize() / 2);
	}
*/

}