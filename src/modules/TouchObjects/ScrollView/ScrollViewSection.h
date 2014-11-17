//
//  ScrollViewSection.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#pragma once

#include "BaseTouchObject.h"

namespace touchObject {

	typedef std::shared_ptr<class		ScrollViewSection>		ScrollViewSectionRef;
	typedef std::shared_ptr<const class ScrollViewSection>		ScrollViewSectionConstRef;
	typedef std::weak_ptr  <class		ScrollViewSection>		ScrollViewSectionWeakRef;


	class ScrollViewSection : public touchObject::BaseTouchObject{
	public:

		ScrollViewSection();
		static ScrollViewSectionRef create( cinder::Vec2f size);
		virtual void draw();

		float getTopInset(){ return mTopInset; };
		float getBottomInset(){ return mBottomInset; };
		float getLeftInset(){ return mLeftInset; };
		float getRightInset(){ return mRightInset; };

		int getSectionTag(){ return mSectionTag; };
		void setSectionTag(int tag){  mSectionTag=tag; };

	
		void setImage(cinder::gl::TextureRef texRef){ mImageTextureRef = texRef; };
	protected:
		float mTopInset, mBottomInset, mLeftInset, mRightInset;
		int mSectionTag;
		cinder::gl::TextureRef mImageTextureRef;

			
	};
}