//
//  ScrollView.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.



#pragma once
#include "BaseTouchObject.h"
#include "ScrollViewCell.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Fbo.h"
namespace touchObject {

	typedef std::shared_ptr<class		ScrollView>		ScrollViewRef;
	typedef std::shared_ptr<const class ScrollView>		ScrollViewConstRef;
	typedef std::weak_ptr  <class		ScrollView>		ScrollViewWeakRef;



	class ScrollView : public touchObject::BaseTouchObject{
	public:

		/**
		* Controls the orientation the in which cells move
		*/
		enum ScrollViewOrientation { Vertical, Horizontal /*,MultiDirectional*/ };
		
		/**
		*	Continous will will allow cells to wrap around
		*	NonContinous will stop scrolling when an end is reached
		*/
		enum ScrollViewType { Continuous, NonContinuous };


		//Creation functions
		ScrollView();
		static ScrollViewRef create(cinder::Vec2f pos, cinder::Vec2f size, ScrollViewType scrollType, ScrollViewOrientation scrollOrientation);
		void  addSection(ScrollViewCellRef section);

		
		virtual void update();
		virtual void draw();

		//Properties
		
		/**
		* Controls how far the view is scrolled in relation to input movement
		*/
		void  setTouchMulitplier(float multiplier){ mTouchMultiplier = multiplier; };
		float getTouchMultiplier(){ return mTouchMultiplier; };
		
		/**
		*	Controls how far the view continues to scroll after input has eneded
		*/
		void  setMomentum(float momentum){ mMomentum = momentum; };
		float getMomentum(){ return mMomentum; };


		void  setScrollPercentage(float targetPercentage);
		float getScrollPercentage();

		
		/**
		* Default is TRUE, Subviews are drawn to an fbo on update.
		*
		* Setting clip subviews to FALSE should give a performance boost
		* since it does not need to render to a fbo on the update. One case
		* could be if the scrollview takes up whole screen and cells that
		* are outside of the view wont be seen anyway.
		*/
		void setShouldClipSubviews(bool state){ mShouldClipSubviews = state; };

	protected:

		//Override base class touch B/M/E handlers to say how we are going to listen to touches and how the object will respond
		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){};

		virtual void render();
		virtual void layoutSections();
		/**
		*	Calculates the width and height of all the cell combined with the cell spacing
		*/
		ci::Vec2f getContentSize();
		void  setBreaklines();
		
		void  repositionSections(float offsetAmt);
		
		
		void  updateSections_Vertical_Continuous(float offsetAmt);
		void  updateSections_Vertical_NonContinuous(float offsetAmt);
		void  updateSections_Horiz_Continuous(float offsetAmt);
		void  updateSections_Horiz_NonContinuous(float offsetAmt);

		void  popSection(ScrollViewCellRef section, bool top);

		ScrollViewCellRef getNewSectionPointer(ScrollViewCellRef section, bool previous);
		void drawBreakLines();


		//Properties

		ScrollViewOrientation mScrollViewOrientation;
		ScrollViewType mScrollViewType;
		bool mShouldClipSubviews;


		//Movement
		cinder::Vec2f	mCurrentTouchPosition, 
						mPreviousTouchPosition;
	
		float mTouchMultiplier;
		float mMomentum;
		cinder::Anim<float> mUpdateAmount;
		float mCurrentOffset;

		//Layout
		float	mBreakLineFront,
				mBreakLineBack;
				

	
		ScrollViewCellRef	 mTopSection, 
							 mBottomSection,
						 	 mLeftSection, 
							 mRightSection;	

		
		std::vector<ScrollViewCellRef> mScrollViewCells;

		//Used for Masking the view 
		cinder::gl::Fbo				mFbo;
		
	};
}