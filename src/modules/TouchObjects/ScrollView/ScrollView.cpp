//
//  TileScrollView.cpp
//  cinder_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#include "ScrollView.h"
#include "TouchManager.h"


using namespace std;
using namespace ci;
using namespace ci::app;
namespace touchObject {
	ScrollView::ScrollView() : BaseTouchObject(),
	mCurrentTouchPosition(Vec2f::zero()),
	mPreviousTouchPosition(Vec2f::zero()),
	mUpdateAmount(0.0f),
	mCurrentOffset(0.0f),
	mMomentum(1.0f),
	mScrollViewType(Continuous),
	mTouchMultiplier(0.050f),
	mVerticalCellSpacing(0.0f),
	mHorizontalCellSpacing (0.0f),
	mShouldClipSubviews(true)
	{

		
	};

	ScrollViewRef ScrollView::create(Vec2f pos, Vec2f size, ScrollViewType scrollType, ScrollViewOrientation scrollOrientation){

		ScrollViewRef scrollViewRef(new ScrollView());

		scrollViewRef->registerWithTouchMngr();

		scrollViewRef->setPosition(pos);
		scrollViewRef->setSize(size);

		scrollViewRef->mScrollViewOrientation = scrollOrientation;
		scrollViewRef->mScrollViewType = scrollType;


		//Setup Fbo
		gl::Fbo::Format format;
		format.setSamples( 4 ); // uncomment this to enable 4x antialiasing
		scrollViewRef->mFbo = gl::Fbo(size.x, size.y, format);
		
		return scrollViewRef;
	}


	void ScrollView::addSection(ScrollViewCellRef section){
		section->setSectionTag(mScrollViewCells.size());
		mScrollViewCells.push_back(section);
	

		setBreaklines();
		layoutSections();
	}
	
	
	Vec2f  ScrollView::getContentSize(){
		
			Vec2f contentSize = Vec2f::zero();
			
				for (ScrollViewCellRef cell : mScrollViewCells){
					contentSize.x += cell->getWidth() + mHorizontalCellSpacing;
					contentSize.y += cell->getHeight() + mVerticalCellSpacing;
				}
				contentSize.x -= mHorizontalCellSpacing;
				contentSize.y -= mVerticalCellSpacing;

			return contentSize;
	}

	//This sets lines where cell will decide If they need be repositioned on the opposite end of the stack
	void  ScrollView::setBreaklines(){

		Vec2f contentSize = getContentSize();
		if (mScrollViewOrientation == Horizontal){
			if (mScrollViewType == Continuous){

				mBreakLineFront = getCenter().x - (contentSize.x / 2);
				mBreakLineBack = getCenter().x + (contentSize.x / 2);
			}
			else{

				mBreakLineFront = getPosition().x;
				mBreakLineBack = getSize().x;

			}
		}
		else{
			float screenSize = getHeight();
			mBreakLineFront = getPosition().y + ((getHeight() / 2) - (contentSize.y / 2));
			mBreakLineBack = getPosition().y + ((getHeight() / 2) + (contentSize.y / 2));
		}


		mLeftSection = mTopSection = mScrollViewCells.front();
		mRightSection = mBottomSection = mScrollViewCells.back();
	}


	


	void ScrollView::layoutSections(){

		//set top and bottom section pointers- thsese keep track of where the top and bototm of the sections stack is 


		if (mScrollViewOrientation == Horizontal){

			float curPos = mBreakLineFront;//Left break
			for (ScrollViewCellRef section : mScrollViewCells){
				
				float sectionWidth = section->getWidth();
				float sectionLeftInset = section->getLeftInset();
				float ypos = getCenter().y - section->getHeight() / 2;

				section->setPosition(Vec2f(curPos, ypos));
				curPos += sectionWidth - sectionLeftInset;
				curPos += mHorizontalCellSpacing;
			}
		}
		else if (mScrollViewOrientation == Vertical){

			float curPos = mBreakLineFront;
			for (ScrollViewCellRef section : mScrollViewCells){
				
				float sectionHeight = section->getHeight();
				float sectionTopInset = section->getTopInset();

				section->setPosition(Vec2f(getPosition().x, curPos - sectionTopInset));
				curPos += sectionHeight - sectionTopInset;
			}
		}
	}




	void	ScrollView::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		//keep track of the touch position so we can later test how far it has moved
		mPreviousTouchPosition = mCurrentTouchPosition = touchPnt;
		mUpdateAmount = 0.0f;

	}
	void	ScrollView::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType){
		mPreviousTouchPosition = mCurrentTouchPosition;
		mCurrentTouchPosition = touchPnt;

		Vec2f offsetAmount = mCurrentTouchPosition - mPreviousTouchPosition;//Get the difference between the old touch point and the moved touch point

		if (mScrollViewOrientation == Horizontal)            mUpdateAmount.value() += floor(offsetAmount.x);//have the sections update their positions
		else if (mScrollViewOrientation == Vertical)         mUpdateAmount.value() += floor(offsetAmount.y);//have the sections update their positions
	
	}
	


	void ScrollView::setScrollPercentage(float targetPercentage){
		float sectionSize, breaklineDistance;

		float currentScrollPos = getScrollPercentage();
		float percentageDifference = currentScrollPos - targetPercentage;

		//console()<<"percentageDifference "<< percentageDifference<<endl;

		if (mScrollViewOrientation == Horizontal){
			sectionSize = mScrollViewCells.front()->getWidth();
			breaklineDistance = mBreakLineBack - mBreakLineFront;

		}
		else{
			sectionSize = mScrollViewCells.front()->getHeight();
			breaklineDistance = mBreakLineBack - mBreakLineFront;

		}
		float endPostion = (mScrollViewCells.size()*sectionSize) - (breaklineDistance);

		//get An update amount
		float updateAmount = percentageDifference*endPostion;
		console() << "UpdateAmount " << updateAmount << endl;
		repositionSections(updateAmount);

	}


	float ScrollView::getScrollPercentage(){
		//user the page id number as an index
		float sectionSize, currentOffset, endOffset, breaklineDistance;


		if (mScrollViewOrientation == Horizontal){
			sectionSize = mScrollViewCells.front()->getWidth();
			currentOffset = mScrollViewCells.front()->getPosition().x - mBreakLineFront;
			breaklineDistance = mBreakLineBack - mBreakLineFront;
			endOffset = (mScrollViewCells.size()*sectionSize) - (breaklineDistance);
		}
		else{
			sectionSize = mScrollViewCells.front()->getHeight();
			currentOffset = mScrollViewCells.front()->getPosition().y - mBreakLineFront;
			breaklineDistance = mBreakLineBack - mBreakLineFront;
			endOffset = -((mScrollViewCells.size()*sectionSize) - (breaklineDistance));
		}

		return currentOffset / endOffset;
	}

	void ScrollView::update(){
		if (mUpdateAmount != 0){
			repositionSections(mUpdateAmount* mTouchMultiplier);
			//Gradually slow scrolling
			timeline().apply(&mUpdateAmount, 0.0f, mMomentum, EaseOutExpo());
		
		

		}
		if (mShouldClipSubviews){
			render();
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
		ScrollViewCellRef section;


		if (offsetAmt < 0){//If scrolling up
			section = mBottomSection;//start with the bottom most section and work up 
		}
		else{//Scrolling Down
			section = mTopSection;//Start with the topmost section and work down
		}

		mCurrentOffset += offsetAmt;
		for (auto cell : mScrollViewCells ){
			float xPos = section->getPosition().x;
			float yPos = section->getPosition().y + offsetAmt;
			section->setPosition(Vec2f(xPos, yPos));

			float top = section->getPosition().y;
			float bottom = section->getPosition().y + section->getHeight();

			//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (top  > mBreakLineBack)popSection(section, true);//section->top
			else if ((bottom) < mBreakLineFront)popSection(section, false);//section ->bottom

			//Get the next Section
			if (offsetAmt < 0)section = getNewSectionPointer(section, true);//Work our way up from the bottom
			else		   section = getNewSectionPointer(section, false);//Work our way down the sections
		}

	}

	void  ScrollView::updateSections_Vertical_NonContinuous(float offsetAmt){


		ScrollViewCellRef cell;
		if (offsetAmt<0){//If scrolling up
			cell = mBottomSection;//start with the bottom most section and work up 


			//check if the right most sections position + width is past the right break point
			//get the distance from the rightsight of the rightmost section to the rightbreak point
			float sectionBottomEdge = cell->getPosition().y + cell->getHeight();
			float dist = sectionBottomEdge - mBreakLineBack;
			if (abs(offsetAmt)>dist) offsetAmt = -dist;

		}
		else{

			cell = mTopSection;//Start with the topmost section and work down

			//check if the left most sections position  is past the  left break point
			float dist = (mBreakLineFront - cell->getPosition().y);
			if (offsetAmt > dist) offsetAmt = dist;

		}

		mCurrentOffset += offsetAmt;
		for (auto cell : mScrollViewCells){
			float xPos = cell->getPosition().x;
			float yPos = cell->getPosition().y + offsetAmt;
			cell->setPosition(Vec2f(xPos, yPos));

			float top = cell->getPosition().y;
			float bottom = cell->getPosition().y + cell->getHeight();

			//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (top  > mBreakLineBack)popSection(cell, true);//section->top
			else if ((bottom) < mBreakLineFront)popSection(cell, false);//section ->bottom

			//Get the next Section
			if (offsetAmt < 0)cell = getNewSectionPointer(cell, true);//Work our way up from the bottom
			else		   cell = getNewSectionPointer(cell, false);//Work our way down the sections
		}


	}
	void  ScrollView::updateSections_Horiz_Continuous(float offsetAmt){

		ScrollViewCellRef cell;
		if (offsetAmt < 0){//If scrolling left 
			cell = mRightSection;//start with the bottom most section and work up 
		}
		else{//Scrolling right
			cell = mLeftSection;//Start with the topmost section and work down
		}

		mCurrentOffset += offsetAmt;
		for (auto c : mScrollViewCells){

			float xPos = cell->getPosition().x + offsetAmt;
			float yPos = cell->getPosition().y;
			cell->setPosition(Vec2f(xPos, yPos));


			float left = cell->getPosition().x;
			float right = cell->getPosition().x + cell->getWidth();

			//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (left > mBreakLineBack)			popSection(cell, true);//section->top
			else if ((right) < mBreakLineFront)	popSection(cell, false);//section ->bottom


			//Get the next Section to reposition
			if (offsetAmt < 0)cell = getNewSectionPointer(cell, true);//Work our way up from the bottom
			else		   cell = getNewSectionPointer(cell, false);//Work our way down the sections

		}
	}

	void  ScrollView::updateSections_Horiz_NonContinuous(float offsetAmt){
		ScrollViewCellRef cell;
		if (offsetAmt<0){//If scrolling left 
			cell = mRightSection;//start with the bottom most section and work up 


			//check if the right most sections position + width is past the right break point
			//get the distance from the rightsight of the rightmost section to the rightbreak point
			float sectionRightEdge = cell->getPosition().x + cell->getWidth();
			float dist = sectionRightEdge - mBreakLineBack;
			if (abs(offsetAmt)>dist) offsetAmt = -dist;

		}
		else{//Scrolling right

			cell = mLeftSection;//Start with the topmost section and work down

			//check if the left most sections position  is past the  left break point
			float dist = (mBreakLineFront - cell->getPosition().x);
			if (offsetAmt > dist) offsetAmt = dist;

		}

		mCurrentOffset += offsetAmt;
		for (auto cell : mScrollViewCells){

			float xPos = cell->getPosition().x + offsetAmt;
			float yPos = cell->getPosition().y;
			cell->setPosition(Vec2f(xPos, yPos));


			//Get the next Section to reposition
			if (offsetAmt < 0)cell = getNewSectionPointer(cell, true);//Work our way up from the bottom
			else		   cell = getNewSectionPointer(cell, false);//Work our way down the sections

		}
	}




	void ScrollView::popSection(ScrollViewCellRef cell, bool top){
		if (mScrollViewOrientation == Horizontal){
			if (top){//Section->TOP

				float xPos = (mLeftSection->getPosition().x + mLeftSection->getLeftInset());
				//Subtract the sections height
				float width = cell->getWidth();
				xPos = xPos - width;

				//section->Position.value().y =yPos;
				cell->setPosition(Vec2f(xPos, cell->getPosition().y));

				//Set this section as New Top Section 
				mLeftSection = cell;

				//Set the new bottom Section
				mRightSection = getNewSectionPointer(cell, true);// get the previous section

			}
			else{//Section->BOTTOM
				float xPos = mRightSection->getPosition().x + mRightSection->getWidth();

				//section->Position.value().y =yPos - section->getTopInset();
				cell->setPosition(Vec2f(xPos - cell->getLeftInset(), cell->getPosition().y));

				//Set this section as the new bottom section
				mRightSection = cell;

				//Set the new top Section
				mLeftSection = getNewSectionPointer(cell, false);// get the previous section
			}
		}
		else if (mScrollViewOrientation == Vertical){
			if (top){//Section is going to TOP

				float yPos = (mTopSection->getPosition().y + mTopSection->getTopInset());
				//Subtract the sections height
				float height = cell->getHeight();
				yPos = yPos - height;

				//section->Position.value().y =yPos;
				cell->setPosition(Vec2f(cell->getPosition().x, yPos));

				//Set this section as New Top Section 
				mTopSection = cell;

				//Set the new bottom Section
				mBottomSection = getNewSectionPointer(cell, true);// get the previous section

			}
			else{//Section going to bottom BOTTOM
				float yPos = mBottomSection->getPosition().y + mBottomSection->getHeight();

				//section->Position.value().y =yPos - section->getTopInset();
				cell->setPosition(Vec2f(cell->getPosition().x, yPos - cell->getTopInset()));

				//Set this section as the new bottom section
				mBottomSection = cell;

				//Set the new top Section
				mTopSection = getNewSectionPointer(cell, false);// get the previous section
			}
		}
	}

	ScrollViewCellRef ScrollView::getNewSectionPointer(ScrollViewCellRef section, bool previous){

		ScrollViewCellRef nextSection;
		if (previous){
			int prevIndex = (section->getSectionTag() - 1);
			//if index is below the range of the child objects  vector reset it to the end of the list
			if (prevIndex < 0)	prevIndex = mScrollViewCells.size() - 1;
			nextSection = mScrollViewCells.at(prevIndex);

		}
		else{
			int nextIndex = ((section->getSectionTag()) + 1);
			//if the index is  past the end of the child objects vector set it to the begining 
			if (nextIndex >= mScrollViewCells.size())	nextIndex = 0;
			nextSection = mScrollViewCells.at(nextIndex);
		}

		if (nextSection)return nextSection;
		else return 0;
	}

	void ScrollView::render(){
		Area area = gl::getViewport();
		gl::SaveFramebufferBinding bindingSaver;
		gl::setMatricesWindow(getWindowSize(), false);
			// bind the framebuffer - now everything we draw will go there
			mFbo.bindFramebuffer(); {
				gl::translate(-getPosition());
				

					// clear out the FBO 
					gl::clear(Color(0.0, 0.0, 0.0));
					for (auto section : mScrollViewCells){
						section->draw();
					}
					gl::translate(getPosition());
			}
			gl::setMatricesWindow(getWindowSize(), true);
	}


	void ScrollView::draw(){
		//Draw ScrollView Background
		//draw the sections 
		
		drawDebugBox(true);

		if (mShouldClipSubviews){

			gl::color(1, 1, 1);
			gl::draw(mFbo.getTexture(), getRect());

		}
		else{
			for (ScrollViewCellRef section : mScrollViewCells){
				section->draw();
			}
		}
		
	

		//drawBreakLines();
	}

	void ScrollView::drawBreakLines(){
		Vec2f pos = getPosition();
		gl::lineWidth(2.0f);
		if (mScrollViewOrientation == Horizontal){

			gl::color(0, 1, 0);
			gl::drawLine(Vec2f(mBreakLineFront, 0) + pos, Vec2f(mBreakLineFront, getHeight()) + pos);
			gl::color(1, 0, 0);
			gl::drawLine(Vec2f(mBreakLineBack, 0) + pos, Vec2f(mBreakLineBack, getHeight()) + pos);

		}
		else{
			gl::color(0, 1, 0);
			gl::drawLine(Vec2f(0, mBreakLineFront) + pos, Vec2f(getWidth(), mBreakLineFront) + pos);
			gl::color(1, 0, 0);
			gl::drawLine(Vec2f(0, mBreakLineBack) + pos, Vec2f(getWidth(), mBreakLineBack) + pos);
		}
	}
}