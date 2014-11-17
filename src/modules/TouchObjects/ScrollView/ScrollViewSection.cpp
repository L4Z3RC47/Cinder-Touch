//
//  ScrollViewSection.cpp
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#include "cinder/app/AppNative.h"
#include "ScrollViewSection.h"

using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {

	ScrollViewSection::ScrollViewSection() : BaseTouchObject(),
		mTopInset(0.0f),
		mBottomInset(0.0f),
		mLeftInset(0.0f),
		mRightInset(0.0f),
		mSectionTag(0)
	{};


	ScrollViewSectionRef ScrollViewSection::create(cinder::Vec2f size){
		ScrollViewSectionRef  sectionRef(new ScrollViewSection());

		sectionRef->setSize(size);

		return sectionRef;
	}



	void ScrollViewSection::draw(){

		gl::color(Color(1,1,1));
		gl::draw(mImageTextureRef,getRect());

	}
}