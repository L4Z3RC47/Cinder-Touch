//
//  ScrollViewCell.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#pragma once

#include "BaseTouchObject.h"

namespace touchObject {

	typedef std::shared_ptr<class		ScrollViewCell>		ScrollViewCellRef;
	typedef std::shared_ptr<const class ScrollViewCell>		ScrollViewCellConstRef;
	typedef std::weak_ptr  <class		ScrollViewCell>		ScrollViewCellWeakRef;


	class ScrollViewCell : public touchObject::BaseTouchObject{
	public:

		ScrollViewCell();
		static ScrollViewCellRef create(cinder::Vec2f size);
		virtual void draw();

		void  setTopPadding(float ins){ mPadding.y1 = ins; };
		float getTopPadding(){ return mPadding.y1; };

		void  setBottomPadding(float ins){ mPadding.y2 = ins; };
		float getBottomPadding(){ return mPadding.y2; };
		
		void  setLeftPadding(float ins){ mPadding.x1 = ins; };
		float getLeftPadding(){ return mPadding.x1; };

		void  setRightPadding(float ins){ mPadding.x2 = ins; };
		float getRightPadding(){ return mPadding.x2; };


		float getWidthWithPadding(){ return getWidth()   + getLeftPadding() + getRightPadding(); };
		float getHeightWithPadding(){ return getHeight() + getTopPadding() + getBottomPadding(); };

		void setSectionTag(int tag){  mSectionTag=tag; };
		int  getSectionTag(){ return mSectionTag; };

	
		void setImage(cinder::gl::TextureRef texRef){ mImageTextureRef = texRef; };
	
	protected:

		ci::Rectf mPadding;
		int mSectionTag;
		cinder::gl::TextureRef mImageTextureRef;

			
	};
}