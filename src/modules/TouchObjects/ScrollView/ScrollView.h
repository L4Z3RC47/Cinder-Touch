//
//  ScrollView.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.



#pragma once
#include "BaseTouchObject.h"
#include "ScrollViewCell.h"
#include "cinder/Timeline.h"
namespace touchObject {

	typedef std::shared_ptr<class		ScrollView>		ScrollViewRef;
	typedef std::shared_ptr<const class ScrollView>		ScrollViewConstRef;
	typedef std::weak_ptr  <class		ScrollView>		ScrollViewWeakRef;



	class ScrollView : public touchObject::BaseTouchObject{
	public:
		enum ScrollViewOrientation { Vertical, Horizontal /*,MultiDirectional*/ };
		//Continous will pop sections to the opposite end
		//NonContinous will stop scrolling when an end is reached
		enum ScrollViewType { Continuous, NonContinuous };

		ScrollView();
		static ScrollViewRef create(cinder::Vec2f pos, cinder::Vec2f size, ScrollViewType scrollType, ScrollViewOrientation scrollOrientation);
		virtual void update();
		virtual void draw();

		void  setTouchMulitplier(float multiplier){ mTouchMultiplier = multiplier; };
		float getTouchMultiplier(){ return mTouchMultiplier; };
		
		void  setMomentum(float momentum){ mMomentum = momentum; };
		float getMomentum(){ return mMomentum; };

		void  setScrollPercentage(float targetPercentage);
		void  addSection(ScrollViewCellRef section);
		void  setOffsetPercentageAmount(float offset);
		float getScrollPercentage();
		virtual void  layoutSections();
	protected:

		//Override base class touch B/M/E handlers to say how we are going to listen to touches and how the object will respond
		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);


		void  getTotalSectionSize();
		void  setBreaklines();
		void  setSectionPntrs();
		void  repositionSections(float offsetAmt);
		void  updateSections_Vertical_Continuous(float offsetAmt);
		void  updateSections_Vertical_NonContinuous(float offsetAmt);
		void  updateSections_Horiz_Continuous(float offsetAmt);
		void  updateSections_Horiz_NonContinuous(float offsetAmt);


		void  popSection(ScrollViewCellRef section, bool top);

		ScrollViewCellRef getNewSectionPointer(ScrollViewCellRef section, bool previous);
		void drawBreakLines();



		ScrollViewOrientation mScrollViewOrientation;
		ScrollViewType mScrollViewType;
		cinder::Vec2f mCurrentTouchPosition, mOldTouchPosition;
		bool mTouchMoved;
		float mBreakLineTop, mBreakLineBottom, mBreakLineLeft, mBreakLineRight;
		float mContentSize;
		float mVerticalSectionSpacing, mHorizSectionSpacing;
		float mTouchMultiplier;
		float mMomentum;
		cinder::Anim<float> mUpdateAmount;
		float mCurrentOffset;
		ScrollViewCellRef mTopSection, 
							 mBottomSection,
						 	 mLeftSection, 
							 mRightSection;	

		bool mViewCarriesMomentum;
		
		std::vector<ScrollViewCellRef> mSections;
		int mTotalSections;
	};
}