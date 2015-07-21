
//
//  ScrollViewCellButton.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/15.
//
//


#include "cinder/app/AppNative.h"
#include "ScrollViewCell_W_Button.h"
#include "TouchObjects/ScrollView/ScrollView.h"
#include "TouchManager.h"
using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {

	ScrollViewCell_W_Button::ScrollViewCell_W_Button() :
		mToggled(false)
		
	{
	}

	ScrollViewCell_W_ButtonRef ScrollViewCell_W_Button::create(cinder::Vec2f size){
	
		ScrollViewCell_W_ButtonRef cell_W_ButtonRef = ScrollViewCell_W_ButtonRef(new ScrollViewCell_W_Button);
		cell_W_ButtonRef->setSize(size);
		cell_W_ButtonRef->setDebugString("ScrollViewCell_W_ BTN : " + to_string(cell_W_ButtonRef->getUniqueID()));
		
		//Add a button to the cell
		cell_W_ButtonRef->mButtonRef = BaseButton::create(cinder::Vec2f(0.0f, 0.0f), size / 2.0f, std::bind(&ScrollViewCell_W_Button::buttonCallback, cell_W_ButtonRef, std::placeholders::_1));
		cell_W_ButtonRef->mButtonRef->setDebugString("Cell Button:" + to_string(cell_W_ButtonRef->mButtonRef->getUniqueID()));
		//Connect the Touch canceled function to the button
		cell_W_ButtonRef->mButtonRef->setCancelCallBackFn(std::bind(&ScrollViewCell_W_Button::handleTouchCanceled, cell_W_ButtonRef, std::placeholders::_1, std::placeholders::_2));

	
		return cell_W_ButtonRef;
	}

	void ScrollViewCell_W_Button::setPosition(const cinder::Vec2f &pt){
		ScrollViewCell::setPosition(pt);
		//Update child objects parent position here
		mButtonRef->setParentPosition(getGlobalPosition());
	
	}

	void ScrollViewCell_W_Button::buttonCallback(touchObject::TouchObjectRef obj){
		console() << "Button Selected!!!" << endl;
		mToggled = !mToggled;
	}

	void	ScrollViewCell_W_Button::handleTouchCanceled(touchObject::TouchObjectRef obj, int touchId){
		TouchManager::getInstance()->sendTouchToObject(touchId, mParentScrollViewRef);
	}


	void	ScrollViewCell_W_Button::draw(cinder::Vec2f translationOffset){
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