//
//  ScrollViewCell.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#include "cinder/app/AppNative.h"
#include "ScrollViewCell.h"
#include "ScrollView.h"
using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {

	ScrollViewCell::ScrollViewCell() : BaseTouchObject(),
		mPadding(Rectf::zero()){};


	ScrollViewCellRef ScrollViewCell::create(cinder::Vec2f size){
		ScrollViewCellRef  cellRef(new ScrollViewCell());
		return cellRef;
	}


	void	ScrollViewCell::tapEventHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		console() << "Scrollview Cell recivied Tap Event" << endl;
	}


	void	ScrollViewCell::draw(cinder::Vec2f translationOffset){
		gl::pushMatrices(); {
			gl::translate(translationOffset);
			gl::color(Color(1, 1, 1));
		
			drawDebugBox();

		}gl::popMatrices(); 


	}
}