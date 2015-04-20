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
		
	{};


	ScrollViewCellRef ScrollViewCell::create(cinder::Vec2f size){
		ScrollViewCellRef  sectionRef(new ScrollViewCell());

		sectionRef->setSize(size);
		return sectionRef;
	}



	void ScrollViewCell::draw(){
		gl::color(Color(1,1,1));
	
		drawDebugBox(true);
		if (mImageTextureRef){
			gl::draw(mImageTextureRef, getRect());
		}
	}
}