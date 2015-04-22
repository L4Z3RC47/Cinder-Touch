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
		mMomentum(1.50f),
		mScrollViewType(Continuous),
		mTouchMultiplier(0.050f),
		mShouldClipSubviews(true),
		mNeedsLayout(true)
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


	void ScrollView::addCell(ScrollViewCellRef cell){
		if (mScrollViewCells.size() > 0){
		ScrollViewCellRef lastCell = mScrollViewCells.back();
			cell->setPrevCell(lastCell);
			lastCell->setNextCell(cell);
		}
		cell->setParentPosition(getPosition());
		mScrollViewCells.push_back(cell);
	
	}
	
	
	Vec2f  ScrollView::getContentSize(){
		
			Vec2f contentSize = Vec2f::zero();
				for (ScrollViewCellRef cell : mScrollViewCells){
					contentSize.x += cell->getWidthWithPadding();
					contentSize.y += cell->getHeightWithPadding();
				}
			return contentSize;
	}

	

	//This sets lines where cell will decide If they need be repositioned on the opposite end of the stack
	void  ScrollView::setBreaklines(){

		Vec2f contentSize = getContentSize();

			if (mScrollViewType == Continuous){
				console() << "CENTER POINT " << getCenter() << endl;
				float screenCenter = (mScrollViewOrientation == Horizontal) ? getWidth() / 2.0f       : getHeight() / 2.0f;
				float contentHalf  = (mScrollViewOrientation == Horizontal) ? (contentSize.x / 2) : (contentSize.y / 2);
				

				mBreakLineFront = screenCenter - contentHalf;
				mBreakLineBack = screenCenter + contentHalf;
			}
			else{

				mBreakLineFront = 0.0f;// (mScrollViewOrientation == Horizontal) ? getPosition().x : getPosition().y;
				mBreakLineBack = (mScrollViewOrientation == Horizontal) ? ( getSize().x) : ( getSize().y);

			}

		mFrontCell = mScrollViewCells.front();
		mBackCell = mScrollViewCells.back();
	
	}

	
	void ScrollView::layoutSections(){


		float curPos = mBreakLineFront;


		for (ScrollViewCellRef cell : mScrollViewCells){

			if (mScrollViewOrientation == Horizontal){

				cell->setPosition(Vec2f(curPos - cell->getLeftPadding(), getPosition().y));
				curPos += cell->getWidth() + cell->getRightPadding();
			}
			else if (mScrollViewOrientation == Vertical){

				cell->setPosition(Vec2f(getPosition().x, curPos - cell->getTopPadding()));
				curPos += cell->getHeight() + cell->getBottomPadding();
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

       mUpdateAmount.value() += floor((mScrollViewOrientation == Horizontal) ? offsetAmount.x : offsetAmount.y);//have the sections update their positions

	
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
		updateCellPositions(updateAmount);

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
		if (mNeedsLayout){
			setBreaklines();
			layoutSections();
			mNeedsLayout = false;
		}

		if (abs(mUpdateAmount) > 0.01){
			updateCellPositions(mUpdateAmount* mTouchMultiplier);
			//Gradually slow scrolling
			timeline().apply(&mUpdateAmount, 0.0f, mMomentum, EaseOutExpo());
		}
	
		
		if (mShouldClipSubviews){
			render();
		}
		
	}

	void ScrollView::updateCellPositions(float offsetAmt){
	
			if (mScrollViewType == Continuous){
				updateCells_Continuous(offsetAmt);
			}

			if (mScrollViewType == NonContinuous){
				updateCells_NonContinuous(offsetAmt);
			}
	}

	//If we are dragging up: start updating from the bottom of the stack to make sure the order stays correct.
	// this way sections that hit the top break point first are repositined to the bottom first.(Vise versa)
	//before we are arbitratairly updating the section and this did not keep the sections in order


	void  ScrollView::updateCells_Continuous(float offsetAmt){
		ScrollViewCellRef currentCell = (offsetAmt < 0) ? mBackCell : mFrontCell;////If scrolling up start with the bottom most section and work up //If Scrolling Down start with the topmost section and work down
		
		mCurrentOffset += offsetAmt;
		
		for (auto cell : mScrollViewCells ){
			Vec2f cellEdges = updateCellPosition(currentCell, offsetAmt);
				//Check if the section has gone past the bottom set it to the top of the scroll view 
			if (cellEdges[FRONT]  > mBreakLineBack)			{
				popCell(currentCell, true);//current cell goes to FRONT
			}
			else if ((cellEdges[BACK]) < mBreakLineFront){ popCell(currentCell, false); }//Current cell goes to BACK

			//Get the next Cell
			currentCell = (offsetAmt < 0) ? currentCell->getPrevCell() : currentCell->getNextCell();//Work our way up from the bottom else Work our way down the sections
		
		}
	
	}

	void  ScrollView::updateCells_NonContinuous(float offsetAmt){


		ScrollViewCellRef currentCell;


		if (offsetAmt<0){//If scrolling up
			currentCell = mBackCell;//start with the bottom most section and work up 

			//check if the right most sections position + width is past the right break point
			//get the distance from the rightside of the rightmost section to the rightbreak point
			Vec2f cellPosition = currentCell->getPosition();
			float cellBackEdge = 0.0f;
			
			if (mScrollViewOrientation == Vertical){
				cellBackEdge = cellPosition.y + currentCell->getHeight();
			}
			else{
				cellBackEdge = cellPosition.x + currentCell->getWidth();
			}
		

			float dist = cellBackEdge - mBreakLineBack;
			if (abs(offsetAmt)>dist) offsetAmt = -dist;

		}
		else{
			currentCell = mFrontCell;//Start with the topmost section and work down
			Vec2f cellPosition = currentCell->getPosition();

			float cellFrontEdge = 0.0f;
			if (mScrollViewOrientation == Vertical){
				cellFrontEdge = cellPosition.y ;
			}
			else{
				cellFrontEdge = cellPosition.x ;
			}
			//check if the left most sections position  is past the  left break point
			float dist = (mBreakLineFront - cellFrontEdge);
			if (offsetAmt > dist) offsetAmt = dist;

		}

		mCurrentOffset += offsetAmt;


		for (auto cell : mScrollViewCells){
			Vec2f cellEdges = updateCellPosition(currentCell, offsetAmt);

			currentCell = (offsetAmt < 0) ? currentCell->getPrevCell() : currentCell->getNextCell();//Work our way up from the bottom
			

			if (currentCell == NULL)break;
		}


	}


	ci::Vec2f ScrollView::updateCellPosition(ScrollViewCellRef cell, float offsetAmt){
		
		float front = 0.0f;
		float back = 0.0f;
		Vec2f cellPosition = cell->getPosition();

		if (mScrollViewOrientation == Vertical){
			cell->setPosition(Vec2f(cellPosition.x, cellPosition.y + offsetAmt));

			cellPosition = cell->getPosition();
			front = cellPosition.y - cell->getTopPadding();// TOP EDGE
			back = cellPosition.y + cell->getHeight() + cell->getBottomPadding();// BOTTOM EDGE

		}
		else{
			cell->setPosition(Vec2f(cellPosition.x + offsetAmt, cellPosition.y));
			cellPosition = cell->getPosition();

			front = cellPosition.x - cell->getLeftPadding(); //LEFT EDGE
			back = cellPosition.x + cell->getWidth() + cell->getRightPadding();//RIGHT EDGE
		}
	
		return Vec2f(front,back);

	}

	void ScrollView::popCell(ScrollViewCellRef cell, bool toFront){
	
		if (toFront){//cell is moving from BACK  to FRONT
			ScrollViewCellRef frontCell = mFrontCell;
				if (mScrollViewOrientation == Vertical){
					
					//Get the postion of the top cell
					float yPos = (frontCell->getPosition().y - frontCell->getTopPadding());
					//Subtract the cell height and the bottom padding
					yPos -= (cell->getHeight() - cell->getBottomPadding());
					cell->setPosition(Vec2f(cell->getPosition().x, yPos));
				
				}else{

					float xPos = (frontCell->getPosition().x - frontCell->getLeftPadding());
					//Subtract the sections height
					xPos -= cell->getWidth() - cell->getRightPadding();
					cell->setPosition(Vec2f(xPos, cell->getPosition().y));
				}

				//Set the new back cell
				mBackCell = cell->getPrevCell(); // get the previous section
				cell->clearPrevCell();//Clear Prev Cell

				//Set this section as New Top Section 
				cell->setNextCell(mFrontCell);
				mFrontCell->setPrevCell(cell);
				mFrontCell = cell;


			

			}else{//cell is moving from FRONT  to BACK
			ScrollViewCellRef backCell = mBackCell;
				if (mScrollViewOrientation == Vertical){
					float yPos = backCell->getPosition().y + backCell->getHeight() + backCell->getBottomPadding();
					//Set the cell postion to the bottom of the previous cell plus the cell's top padding
					cell->setPosition(Vec2f(cell->getPosition().x, yPos + cell->getTopPadding()));
				}else{

					float xPos = backCell->getPosition().x + backCell->getWidth() + backCell->getRightPadding();
					cell->setPosition(Vec2f(xPos + cell->getLeftPadding(), cell->getPosition().y));
				}


				mFrontCell = cell->getNextCell();
				cell->clearNextCell();
				//Set this section as the new bottom section
				cell->setPrevCell(mBackCell);//set the current cell->previous to back cell
				mBackCell->setNextCell(cell);//Set the back cell next to curren cell
				mBackCell = cell;

			}
		}

	void ScrollView::render(){
		Area area = gl::getViewport();
		gl::SaveFramebufferBinding bindingSaver;
		gl::setMatricesWindow(getWindowSize(), false);
			// bind the framebuffer - now everything we draw will go there
			mFbo.bindFramebuffer(); {
				//gl::translate(-getPosition());
				
					// clear out the FBO 
					gl::clear(Color(0.0, 0.0, 0.0));
					for (auto section : mScrollViewCells){
						section->draw();
					}
					//gl::translate(getPosition());
			}
			gl::setMatricesWindow(getWindowSize(), true);
	}


	void	ScrollView::draw(cinder::Vec2f translationOffset ){

		//Draw ScrollView Background
		gl::pushMatrices(); {
			gl::translate(translationOffset);
			drawDebugBox();

			
			if (mShouldClipSubviews){

				gl::color(1, 1, 1);
				gl::draw(mFbo.getTexture(), getRect(LOCAL));

			}else{
			
				for (ScrollViewCellRef cell : mScrollViewCells){
					cell->draw(getPosition());
				}
			}
		
			drawBreakLines();
		}gl::popMatrices();

	}

	void ScrollView::drawBreakLines(){
	
		Vec2f pos = getRect(LOCAL).getUpperLeft();
		gl::lineWidth(2.0f);
		if (mScrollViewOrientation == Horizontal){

			gl::color(0, 1, 0);
			gl::drawLine(Vec2f(mBreakLineFront, 0)+ pos , Vec2f(mBreakLineFront, getHeight())+pos );
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