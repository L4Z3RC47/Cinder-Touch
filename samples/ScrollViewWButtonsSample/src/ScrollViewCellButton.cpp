
//
//  ScrollViewCellButton.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/15.
//
//


#include "cinder/app/AppNative.h"
#include "ScrollViewCellButton.h"

using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {

	ScrollViewCellButton::ScrollViewCellButton():
		mToggled(false)
		
	{

		console() << "ScrollvieCell Button COnstructor" << endl;
	}

	ScrollViewCellButtonRef ScrollViewCellButton::create(cinder::Vec2f size){
	
		ScrollViewCellButtonRef cellButtonRef = ScrollViewCellButtonRef(new ScrollViewCellButton);
		console() << "Creating CellbuttonRef" << endl;
		cellButtonRef->setSize(size);
		cellButtonRef->mButtonRef = BaseButton::create(cinder::Vec2f(0.0f, 0.0f), size/2.0f, std::bind(&ScrollViewCellButton::buttonCallback, cellButtonRef, std::placeholders::_1));
		return cellButtonRef;
	}
	void		ScrollViewCellButton::setPosition(const cinder::Vec2f &pt){
		ScrollViewCell::setPosition(pt);

		//Update child objects parent position here
		mButtonRef->setParentPosition(getPosition());
	
	}

	void	ScrollViewCellButton::tapEventHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		Vec2f convertedPoint = touchPnt - getParentPosition();
		if (mButtonRef->hasTouchPoint(convertedPoint)){
			mButtonRef->touchesBeganHandler(touchID, convertedPoint, touchType);
			mButtonRef->touchesEndedHandler(touchID, convertedPoint, touchType);
		}

	}

	void ScrollViewCellButton::buttonCallback(touchObject::TouchObjectRef){
		mButtonRef->unRegisterWithTouchMngr();
		console() << "Button Selected!!!" << endl;
		mToggled = !mToggled;
		
	}

	void	ScrollViewCellButton::draw(cinder::Vec2f translationOffset){
		gl::pushMatrices(); {
			gl::translate(translationOffset);
			gl::color(getObjectColor());

			drawDebugBox();
			if (mToggled){
				gl::color(getObjectColor());
				gl::drawSolidRect(getRect(LOCAL));
			}

	
			mButtonRef->draw(getPosition());
		}gl::popMatrices();


	}


}