//
//  ScrollViewCell.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#include "cinder/app/AppNative.h"
#include "ScrollViewCell.h"

using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {

	ScrollViewCell::ScrollViewCell() : BaseTouchObject(),
		mPadding(Rectf::zero())
		
	{
		mToString = "Scroll View Cell";
	};


	ScrollViewCellRef ScrollViewCell::create(cinder::Vec2f size){
		ScrollViewCellRef  cellRef(new ScrollViewCell());
		console() << "CELL POS " << cellRef->getRect(LOCAL).getUpperLeft() << endl;
		//cellRef->registerWithTouchMngr();
		cellRef->setSize(size);

		return cellRef;
	}

	/*
	void ScrollViewCell::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		//keep track of the touch position so we can later test how far it has moved
		//This object only takes one touch
		if (mObjectTouchIDs.empty()){
			mObjectTouchIDs.push_back(touchID);
		}
	}

	void ScrollViewCell::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){

		//if the touch point is out of the space of the button = touch canceled.
		if (!mObjectTouchIDs.empty()){
			int currentTouchId = mObjectTouchIDs.front();
			if ((currentTouchId == touchID) && (!hasTouchPoint(touchPnt))){

				//the button no longer has this touch
				mObjectTouchIDs.clear();

			}
		}
	}

	void ScrollViewCell::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){

		if (!mObjectTouchIDs.empty()){
			int currentTouchId = mObjectTouchIDs.front();
			if (currentTouchId == touchID){
				//clear the touch ids
				mObjectTouchIDs.clear();
			}

		
		}
	}
	*/
	void	ScrollViewCell::draw(cinder::Vec2f translationOffset){
		gl::pushMatrices(); {
			gl::translate(translationOffset);
			gl::color(Color(1, 1, 1));
		
			drawDebugBox();

		}gl::popMatrices(); 


	}
}