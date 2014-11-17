//
//  TileScrollView.cpp
//  cinder_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#include "ScrollView.h"

#include "cinder/Xml.h"
#include "TouchManager.h"


using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {
	ScrollView::ScrollView() : BaseTouchObject(),

	mCurrentTouchPosition(Vec2f::zero()),
	mUpdateAmount(0.0f),
	mCurrentOffset(0.0f),
	mContentSize(0.0f),
	mTotalSections(0.0f),
	mViewCarriesMomentum(false),
	mMomentum(1.0f),
	mScrollViewType(Continuous),
	mTouchMultiplier(0.050f),
	mVerticalSectionSpacing(0.0f),
	mHorizSectionSpacing (0.0f),
	mTouchMoved(false)
	{
	};

	ScrollViewRef ScrollView::create(Vec2f pos, Vec2f size, cinder::Vec2f sectionSpacing, ScrollViewType scrollType, ScrollViewOrientation scrollOrientation){

		ScrollViewRef scrollViewRef(new ScrollView());

		scrollViewRef->registerWithTouchMngr();
		scrollViewRef->mScrollViewOrientation = scrollOrientation;
		scrollViewRef->mScrollViewType = scrollType;

		scrollViewRef->setPosition(pos);
		scrollViewRef->setSize(size);

		scrollViewRef->mVerticalSectionSpacing = sectionSpacing.y;
		scrollViewRef->mHorizSectionSpacing = sectionSpacing.x;
	
		return scrollViewRef;
	}


	void ScrollView::addSection(ScrollViewSectionRef section){
		section->setSectionTag(mTotalSections);
		mSections.push_back(section);
		mTotalSections += 1;
		
		setSectionPntrs();
		getTotalSectionSize();

		setBreaklines();
		layoutSections();

	

	}

	void ScrollView::getTotalSectionSize(){
		
			mContentSize = 0.0f;
			if (mScrollViewOrientation == Horizontal){
				for (ScrollViewSectionRef section : mSections){
					mContentSize += section->getWidth() + mHorizSectionSpacing;
				}
				mContentSize -= mHorizSectionSpacing;
			}
			else{
				for (ScrollViewSectionRef section : mSections){
					mContentSize += section->getHeight() + mVerticalSectionSpacing;
				}
				mContentSize -= mVerticalSectionSpacing;
			}
		
	}

	//This sets lines where sections will decide to be repositioned
	void  ScrollView::setBreaklines(){
		//set brakelines
		if (mScrollViewOrientation == Horizontal){
			if (mScrollViewType == Continuous){

				mBreakLineLeft = getCenter().x - (mContentSize / 2);
				mBreakLineRight = getCenter().x + (mContentSize / 2);
			}
			else{

				mBreakLineLeft = getPosition().x;
				mBreakLineRight =getSize().x;

			}
		}
		else{
			float screenSize = getHeight();
			mBreakLineTop = getPosition().y + ((getHeight() / 2) - (mContentSize / 2));
			mBreakLineBottom = getPosition().y + ((getHeight() / 2) + (mContentSize / 2));
		}

		setSectionPntrs();
	}


	void ScrollView::setSectionPntrs(){
		if (mSections.size() > 0){
			//section pointers
			mLeftSection = mTopSection =mSections.front();
			mRightSection = mBottomSection = mSections.back();
		}
	}


	void ScrollView::layoutSections(){

		//set top and bottom section pointers- thsese keep track of where the top and bototm of the sections stack is 


		if (mScrollViewOrientation == Horizontal){

			//float curPos = getPosition().x;//Left break
			float curPos = mBreakLineLeft;//Left break
			for (ScrollViewSectionRef section : mSections){
				
				float sectionWidth = section->getWidth();
				float sectionLeftInset = section->getLeftInset();
				//curPos += sectionWidth -sectionLeftInset;
				float ypos = getCenter().y - section->getHeight() / 2;

				section->setPosition(Vec2f(curPos, ypos));
				curPos += sectionWidth - sectionLeftInset;
				curPos += mHorizSectionSpacing;
			}


		}
		else if (mScrollViewOrientation == Vertical){

			float curPos = mBreakLineTop;
			for (ScrollViewSectionRef section : mSections){
				
				float sectionHeight = section->getHeight();
				float sectionTopInset = section->getTopInset();

				section->setPosition(Vec2f(getPosition().x, curPos - sectionTopInset));
				curPos += sectionHeight - sectionTopInset;
			}
		}
	}


	////////////////////////////////////////////////////////////
	/*
	void ScrollView::touchesBeganHandler(){
		//keep track of the touch position so we can later test how far it has moved
		mOldTouchPosition = mCurrentTouchPosition = TouchManager::getCurrentTouchPoint();

		mUpdateAmount = 0.0f;

		//keep track if the touch moves
		mTouchMoved = false;
	}

	void ScrollView::touchesMovedHandler(){
		mOldTouchPosition = mCurrentTouchPosition;
		mCurrentTouchPosition = TouchManager::getCurrentTouchPoint();

		Vec2f offsetAmount = mCurrentTouchPosition - mOldTouchPosition;//Get the difference between the old touch point and the moved touch point

		if (mScrollViewOrientation == Horizontal)                mUpdateAmount.value() += floor(offsetAmount.x);//have the sections update their positions
		else if (mScrollViewOrientation == Vertical)         mUpdateAmount.value() += floor(offsetAmount.y);//have the sections update their positions
		if (abs(offsetAmount.x) > 1.0f || abs(offsetAmount.y) > 1.0f)                mTouchMoved = true;
	}

	void ScrollView::touchesEndedHandler(){
		if (!mTouchMoved){
			routeTouchesToChildren();
		}
	}

	*/


	void	ScrollView::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		//keep track of the touch position so we can later test how far it has moved
		mOldTouchPosition = mCurrentTouchPosition = touchPnt;

		mUpdateAmount = 0.0f;

		//keep track if the touch moves
		mTouchMoved = false;
	}
	void	ScrollView::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		mOldTouchPosition = mCurrentTouchPosition;
		mCurrentTouchPosition = touchPnt;

		Vec2f offsetAmount = mCurrentTouchPosition - mOldTouchPosition;//Get the difference between the old touch point and the moved touch point

		if (mScrollViewOrientation == Horizontal)                mUpdateAmount.value() += floor(offsetAmount.x);//have the sections update their positions
		else if (mScrollViewOrientation == Vertical)         mUpdateAmount.value() += floor(offsetAmount.y);//have the sections update their positions
		if (abs(offsetAmount.x) > 1.0f || abs(offsetAmount.y) > 1.0f)                mTouchMoved = true;
	}
	
	
	void	ScrollView::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
	
	}



	void ScrollView::setScrollPercentage(float targetPercentage){
		float sectionSize, breaklineDistance;

		float currentScrollPos = getScrollPercentage();
		float percentageDifference = currentScrollPos - targetPercentage;

		//console()<<"percentageDifference "<< percentageDifference<<endl;

		if (mScrollViewOrientation == Horizontal){
			sectionSize = mSections.front()->getWidth();
			breaklineDistance = mBreakLineRight - mBreakLineLeft;

		}
		else{
			sectionSize = mSections.front()->getHeight();
			breaklineDistance = mBreakLineBottom - mBreakLineTop;

		}
		float endPostion = (mSections.size()*sectionSize) - (breaklineDistance);

		//get An update amount
		float updateAmount = percentageDifference*endPostion;
		console() << "UpdateAmount " << updateAmount << endl;
		repositionSections(updateAmount);

	}


	float ScrollView::getScrollPercentage(){
		//user the page id number as an index
		float sectionSize, currentOffset, endOffset, breaklineDistance;


		if (mScrollViewOrientation == Horizontal){
			sectionSize = mSections.front()->getWidth();
			currentOffset = mSections.front()->getPosition().x - mBreakLineLeft;
			breaklineDistance = mBreakLineRight - mBreakLineLeft;
			endOffset = (mSections.size()*sectionSize) - (breaklineDistance);
		}
		else{
			sectionSize = mSections.front()->getHeight();
			currentOffset = mSections.front()->getPosition().y - mBreakLineTop;
			breaklineDistance = mBreakLineBottom - mBreakLineTop;
			endOffset = -((mSections.size()*sectionSize) - (breaklineDistance));
		}

		return currentOffset / endOffset;
	}
	void ScrollView::update(){
		if (mUpdateAmount != 0){
			repositionSections(mUpdateAmount* mTouchMultiplier);
			//Gradually slow scrolling
			timeline().apply(&mUpdateAmount, 0.0f, mMomentum, EaseOutExpo());
		}
	}

	void ScrollView::repositionSections(float offsetAmt){
		if (mScrollViewOrientation == Horizontal){
			if (mScrollViewType == Continuous){
				updateSections_Horiz_Continuous(offsetAmt);
			}
			if (mScrollViewType == NonContinuous){
				updateSections_Horiz_NonContinuous(offsetAmt);
			}
		}


		if (mScrollViewOrientation == Vertical){
			if (mScrollViewType == Continuous){
				updateSections_Vertical_Continuous(offsetAmt);
			}
			if (mScrollViewType == NonContinuous){
				updateSections_Vertical_NonContinuous(offsetAmt);
			}
		}

	}

	//If we are dragging up: start updating from the bottom of the stack to make sure the order stays correct.
	// this way sections that hit the top break point first are repositined to the bottom first.(Vise versa)
	//before we are arbitratairly updating the section and this did not keep the sections in order


	void  ScrollView::updateSections_Vertical_Continuous(float offsetAmt){
		ScrollViewSectionRef section;


		if (offsetAmt < 0){//If scrolling up
			section = mBottomSection;//start with the bottom most section and work up 
		}
		else{//Scrolling Down
			section = mTopSection;//Start with the topmost section and work down
		}

		mCurrentOffset += offsetAmt;
		for (int i = 0; i<mTotalSections; i++){
			float xPos = section->getPosition().x;
			float yPos = section->getPosition().y + offsetAmt;
			section->setPosition(Vec2f(xPos, yPos));

			float top = section->getPosition().y;
			float bottom = section->getPosition().y + section->getHeight();

			//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (top  > mBreakLineBottom)popSection(section, true);//section->top
			else if ((bottom) < mBreakLineTop)popSection(section, false);//section ->bottom

			//Get the next Section
			if (offsetAmt < 0)section = getNewSectionPointer(section, true);//Work our way up from the bottom
			else		   section = getNewSectionPointer(section, false);//Work our way down the sections
		}

	}

	void  ScrollView::updateSections_Vertical_NonContinuous(float offsetAmt){


		ScrollViewSectionRef section;
		if (offsetAmt<0){//If scrolling up
			section = mBottomSection;//start with the bottom most section and work up 


			//check if the right most sections position + width is past the right break point
			//get the distance from the rightsight of the rightmost section to the rightbreak point
			float sectionBottomEdge = section->getPosition().y + section->getHeight();
			float dist = sectionBottomEdge - mBreakLineBottom;
			if (abs(offsetAmt)>dist) offsetAmt = -dist;

		}
		else{

			section = mTopSection;//Start with the topmost section and work down

			//check if the left most sections position  is past the  left break point
			float dist = (mBreakLineTop - section->getPosition().y);
			if (offsetAmt > dist) offsetAmt = dist;

		}

		mCurrentOffset += offsetAmt;
		for (int i = 0; i<mTotalSections; i++){
			float xPos = section->getPosition().x;
			float yPos = section->getPosition().y + offsetAmt;
			section->setPosition(Vec2f(xPos, yPos));

			float top = section->getPosition().y;
			float bottom = section->getPosition().y + section->getHeight();

			//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (top  > mBreakLineBottom)popSection(section, true);//section->top
			else if ((bottom) < mBreakLineTop)popSection(section, false);//section ->bottom

			//Get the next Section
			if (offsetAmt < 0)section = getNewSectionPointer(section, true);//Work our way up from the bottom
			else		   section = getNewSectionPointer(section, false);//Work our way down the sections
		}


	}
	void  ScrollView::updateSections_Horiz_Continuous(float offsetAmt){

		ScrollViewSectionRef section;
		if (offsetAmt < 0){//If scrolling left 
			section = mRightSection;//start with the bottom most section and work up 

		}
		else{//Scrolling right
			section = mLeftSection;//Start with the topmost section and work down
		}

		mCurrentOffset += offsetAmt;
		for (int i = 0; i < mTotalSections; i++){

			float xPos = section->getPosition().x + offsetAmt;
			float yPos = section->getPosition().y;
			section->setPosition(Vec2f(xPos, yPos));


			float left = section->getPosition().x;
			float right = section->getPosition().x + section->getWidth();

			//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (left > mBreakLineRight)			popSection(section, true);//section->top
			else if ((right) < mBreakLineLeft)	popSection(section, false);//section ->bottom


			//Get the next Section to reposition
			if (offsetAmt < 0)section = getNewSectionPointer(section, true);//Work our way up from the bottom
			else		   section = getNewSectionPointer(section, false);//Work our way down the sections

		}
	}

	void  ScrollView::updateSections_Horiz_NonContinuous(float offsetAmt){
		ScrollViewSectionRef section;
		if (offsetAmt<0){//If scrolling left 
			section = mRightSection;//start with the bottom most section and work up 


			//check if the right most sections position + width is past the right break point
			//get the distance from the rightsight of the rightmost section to the rightbreak point
			float sectionRightEdge = section->getPosition().x + section->getWidth();
			float dist = sectionRightEdge - mBreakLineRight;
			if (abs(offsetAmt)>dist) offsetAmt = -dist;

		}
		else{//Scrolling right

			section = mLeftSection;//Start with the topmost section and work down

			//check if the left most sections position  is past the  left break point
			float dist = (mBreakLineLeft - section->getPosition().x);
			if (offsetAmt > dist) offsetAmt = dist;

		}

		mCurrentOffset += offsetAmt;
		for (int i = 0; i < mTotalSections; i++){

			float xPos = section->getPosition().x + offsetAmt;
			float yPos = section->getPosition().y;
			section->setPosition(Vec2f(xPos, yPos));


			//Get the next Section to reposition
			if (offsetAmt < 0)section = getNewSectionPointer(section, true);//Work our way up from the bottom
			else		   section = getNewSectionPointer(section, false);//Work our way down the sections

		}
	}




	void ScrollView::popSection(ScrollViewSectionRef section, bool top){
		if (mScrollViewOrientation == Horizontal){
			if (top){//Section->TOP

				float xPos = (mLeftSection->getPosition().x + mLeftSection->getLeftInset());
				//Subtract the sections height
				float width = section->getWidth();
				xPos = xPos - width;

				//section->Position.value().y =yPos;
				section->setPosition(Vec2f(xPos, section->getPosition().y));

				//Set this section as New Top Section 
				mLeftSection = section;

				//Set the new bottom Section
				mRightSection = getNewSectionPointer(section, true);// get the previous section

			}
			else{//Section->BOTTOM
				float xPos = mRightSection->getPosition().x + mRightSection->getWidth();

				//section->Position.value().y =yPos - section->getTopInset();
				section->setPosition(Vec2f(xPos - section->getLeftInset(), section->getPosition().y));

				//Set this section as the new bottom section
				mRightSection = section;

				//Set the new top Section
				mLeftSection = getNewSectionPointer(section, false);// get the previous section
			}
		}
		else if (mScrollViewOrientation == Vertical){
			if (top){//Section is going to TOP

				float yPos = (mTopSection->getPosition().y + mTopSection->getTopInset());
				//Subtract the sections height
				float height = section->getHeight();
				yPos = yPos - height;

				//section->Position.value().y =yPos;
				section->setPosition(Vec2f(section->getPosition().x, yPos));

				//Set this section as New Top Section 
				mTopSection = section;

				//Set the new bottom Section
				mBottomSection = getNewSectionPointer(section, true);// get the previous section

			}
			else{//Section going to bottom BOTTOM
				float yPos = mBottomSection->getPosition().y + mBottomSection->getHeight();

				//section->Position.value().y =yPos - section->getTopInset();
				section->setPosition(Vec2f(section->getPosition().x, yPos - section->getTopInset()));

				//Set this section as the new bottom section
				mBottomSection = section;

				//Set the new top Section
				mTopSection = getNewSectionPointer(section, false);// get the previous section
			}
		}
	}

	ScrollViewSectionRef ScrollView::getNewSectionPointer(ScrollViewSectionRef section, bool previous){

		ScrollViewSectionRef nextSection;
		if (previous){
			int prevIndex = (section->getSectionTag() - 1);
			//if index is below the range of the child objects  vector reset it to the end of the list
			if (prevIndex < 0)	prevIndex = mSections.size() - 1;
			nextSection = mSections.at(prevIndex);

		}
		else{
			int nextIndex = ((section->getSectionTag()) + 1);
			//if the index is  past the end of the child objects vector set it to the begining 
			if (nextIndex >= mTotalSections)	nextIndex = 0;
			nextSection = mSections.at(nextIndex);
		}

		if (nextSection)return nextSection;
		else return 0;
	}


	void ScrollView::draw(){
		//Draw ScrollView Background


		//draw the sections 

		for (ScrollViewSectionRef section : mSections){
			section->draw();
		}


		drawBreakLines();
	}

	void ScrollView::drawBreakLines(){
		Vec2f pos = getPosition();
		gl::lineWidth(2.0f);
		if (mScrollViewOrientation == Horizontal){

			gl::color(0, 1, 0);
			gl::drawLine(Vec2f(mBreakLineLeft, 0) + pos, Vec2f(mBreakLineLeft, getHeight()) + pos);
			gl::color(1, 0, 0);
			gl::drawLine(Vec2f(mBreakLineRight, 0) + pos, Vec2f(mBreakLineRight, getHeight()) + pos);

		}
		else{
			gl::color(0, 1, 0);
			gl::drawLine(Vec2f(0, mBreakLineTop) + pos, Vec2f(getWidth(), mBreakLineTop) + pos);
			gl::color(1, 0, 0);
			gl::drawLine(Vec2f(0, mBreakLineBottom) + pos, Vec2f(getWidth(), mBreakLineBottom) + pos);
		}
	}
}