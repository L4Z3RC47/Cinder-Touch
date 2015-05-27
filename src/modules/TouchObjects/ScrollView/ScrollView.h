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
		enum ScrollViewEnd { FRONT = 0, BACK };

		//Creation functions
		ScrollView();
		static ScrollViewRef create(cinder::Vec2f pos, cinder::Vec2f size, ScrollViewType scrollType, ScrollViewOrientation scrollOrientation);
		void  addCell(ScrollViewCellRef cell);

		
		virtual void update();
		virtual void draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());


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

		virtual void setupFbo();

		//Override base class touch B/M/E handlers to say how we are going to listen to touches and how the object will respond
		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);

		virtual void render();
		virtual void layoutSections();

		virtual void passTouchToCells();
		/**
		*	Calculates the width and height of all the cell combined with the cell spacing
		*/
		ci::Vec2f getContentSize();
		void  setBreaklines();
		
		void  updateCellPositions(float offsetAmt);
		
		
		void  updateCells_Continuous(float offsetAmt);
		void  updateCells_NonContinuous(float offsetAmt);
		
		/**
		*	Updates a cells position based on what the type of scrollview it is.
		*	Returns the Front and Back Edges of the cellin a Vec2f
		*/
		ci::Vec2f  updateCellPosition(ScrollViewCellRef cell, float offsetAmt);
		void  popCell(ScrollViewCellRef section, bool toFront);

		void drawBreakLines();


		//Properties

		ScrollViewOrientation mScrollViewOrientation;
		ScrollViewType mScrollViewType;
		bool mShouldClipSubviews;
		float mTapMovementThreshold; 

		//Movement
		cinder::Vec2f	mInitalTouchPosition,
						mCurrentTouchPosition, 
						mPreviousTouchPosition;
	
		float mTouchMultiplier;
		float mMomentum;
		cinder::Anim<float> mUpdateAmount;
		float mCurrentOffset;

		//Layout
		float	mBreakLineFront,
				mBreakLineBack;
				
		bool mNeedsLayout;
	
		ScrollViewCellRef	 mFrontCell,
							 mBackCell;
					

		
		std::vector<ScrollViewCellRef> mScrollViewCells;

		//Used for Masking the view 
		cinder::gl::Fbo				mFbo;
		
	};
}