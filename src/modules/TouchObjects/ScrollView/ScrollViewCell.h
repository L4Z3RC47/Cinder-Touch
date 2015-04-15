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

		void  setTopInset(float ins){ mInsets.y1 = ins; };
		float getTopInset(){return mInsets.y1; };

		void  setBottomInset(float ins){ mInsets.y2 = ins; };
		float getBottomInset(){ return mInsets.y2; };
		
		void  setLeftInset(float ins){ mInsets.x1 = ins; };
		float getLeftInset(){return mInsets.x1; };

		void  setRightInset(float ins){ mInsets.x2 = ins; };
		float getRightInset(){return mInsets.x2; };

		void setSectionTag(int tag){  mSectionTag=tag; };
		int  getSectionTag(){ return mSectionTag; };

	
		void setImage(cinder::gl::TextureRef texRef){ mImageTextureRef = texRef; };
	
	protected:

		ci::Rectf mInsets;
		int mSectionTag;
		cinder::gl::TextureRef mImageTextureRef;

			
	};
}