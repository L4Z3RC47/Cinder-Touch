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
		//void setParentScrollview(touchObject::ScrollViewWeakRef scrollViewRef){ mParentScrollViewRef = scrollViewRef; };

		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());


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
	
		void setPrevCell(ScrollViewCellRef cell){ mPrevCell = cell; };
		ScrollViewCellRef getPrevCell(){ return mPrevCell; };
		void clearPrevCell(){ mPrevCell.reset(); };

		void setNextCell(ScrollViewCellRef cell){ mNextCell = cell; };
		ScrollViewCellRef getNextCell(){ return mNextCell; };
		void clearNextCell(){ mNextCell.reset(); };
	
		
		virtual	void	touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){ ci::app::console() << "WARNING::ScrollViewCells Cannot Receive Normal Touch Events. See TapEventHandler" << std::endl; };
		virtual void	touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){ ci::app::console() << "WARNING::ScrollViewCells Cannot Receive Normal Touch Events. See TapEventHandler" << std::endl; };
		virtual void	touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){ ci::app::console() << "WARNING::ScrollViewCells Cannot Receive Normal Touch Events. See TapEventHandler" << std::endl; };
		
		virtual void	tapEventHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
	protected:

		ci::Rectf mPadding;
	
		ScrollViewCellRef mPrevCell,mNextCell;

		//touchObject::ScrollViewWeakRef mParentScrollViewRef

	};
}