#include "PanZoomView.h"

#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "cinder/Timeline.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace touchObject{

	PanZoomView::PanZoomView() :
		mPreviousTouchPosition(Vec2f::zero()),
		mCurrentTouchPosition(Vec2f::zero()),
		mTouchTimeStamp(0),
		mInitalTouchDistance(0.0f),
		mCurrentTouchDistance(0.0f),

		mOffsetUpdateAmount(Vec2f::zero()),
		mMomentumUpdateAmount(Vec2f::zero()),
		mScaleUpdateAmount(0.0f),

		mContentInsets(Rectf::zero()),
		mScrollBounceEnabled(true),
		mZoomBouceEnabled(true),
		mTracking(false),
		mZooming(false),
		mDecelerationRate(.50)
	{

	}

	PanZoomView::~PanZoomView(){
	}

	//Creation Functions

	PanZoomViewRef PanZoomView::create(Vec2f pos, Vec2f size){

		PanZoomViewRef panZoomViewRef(new PanZoomView());

		panZoomViewRef->registerWithTouchMngr();
		panZoomViewRef->setPosition(pos);
		panZoomViewRef->setSize(size);

	
		//Setup Fbo
		gl::Fbo::Format format;
		format.setSamples(4); // uncomment this to enable 4x antialiasing
		panZoomViewRef->mFbo = gl::Fbo(size.x, size.y, format);

		return panZoomViewRef;
	}

	void	PanZoomView::addObject(PanZoomObjectRef pzObject){
		mPanZoomObject = pzObject;
		mPanZoomObject->setParentPosition(getPosition());
		
		//Set min scale
		float newScale = mPanZoomObject->isWidthGreaterThanHeight() ? (getWidth() / mPanZoomObject->getWidth()) : (getHeight() / mPanZoomObject->getHeight());
		console() << "Original Min Scale was : " << mPanZoomObject->getMinScale() << "New scale will be " << newScale << endl;
		mPanZoomObject->setMinScale(Vec2f(newScale, newScale));
		mPanZoomObject->setScale(Vec2f(newScale, newScale));
	};

	//External interaction functions

	void	PanZoomView::setContentOffset(ci::Vec2f pnt, bool animated ){

		if (animated){
			timeline().appendTo(&mMomentumUpdateAmount, pnt, 1.0f).updateFn(std::bind(&PanZoomView::momentumUpdateFn, this));
		}else{
			mPanZoomObject->setPosition(pnt);
		}
	}
	void PanZoomView::momentumUpdateFn(){
		mOffsetUpdateAmount = mOffsetUpdateAmount.value() + mMomentumUpdateAmount;
	}
	void	PanZoomView::zoomToScale(float scale, bool animated){}

	void    PanZoomView::zoomToPoint(const cinder::Vec2f &point, const cinder::Vec2f &scale, bool zoomOutAndIn){}

	void    PanZoomView::resetContent(){}






	void PanZoomView::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
	

		//Only accept 2 Touches
		if (mTouchMap.size() < 2){


			addTouch(touchID, touchPnt);

			//update touch point tracking variables - if the first one is not taken, create touch point 1, otherwise create touch point 2
			if (mTouchMap.size() == 1){
				mPreviousTouchPosition = mCurrentTouchPosition = touchPnt;
				mTouchTimeStamp = getElapsedSeconds();
			
			}else if (mTouchMap.size() == 2){

				mCurrentTouchPosition = getTouchesMidpoint();
				mInitalTouchDistance = mCurrentTouchDistance = getTouchesDistance();
			}

		}

	}

	void PanZoomView::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
		//    console() << "Object::touchesMovedHandler" << endl;

		//keep track of the 2 touch points, update the positions
		map<int, cinder::Vec2f>::iterator it = mTouchMap.find(touchID);
		if (it != mTouchMap.end())	it->second = touchPnt;//Touch Id found;
		else						touchesBeganHandler(touchID, touchPnt, touchType);
		

		if (mTouchMap.size() == 2){//Scaling
			//now we just focus on scale changes
			float mPreviousTouchDistance = mCurrentTouchDistance;
			mCurrentTouchDistance = getTouchesDistance();
			if (mCurrentTouchDistance>10){
				console() << "Inital DISTANCE DIFFERNECE :" << mInitalTouchDistance - mCurrentTouchDistance << endl;
				console() << "DISTANCE DIFFERNECE :" << 1 - (mPreviousTouchDistance/mCurrentTouchDistance ) << endl;
				float updateAmt = 1 - (mPreviousTouchDistance / mCurrentTouchDistance);

				mScaleUpdateAmount = (1 - (mPreviousTouchDistance / mCurrentTouchDistance))*0.25f;
			}


		}
			//not pinching, just one finger moving things around
		else if (mTouchMap.size() == 1 ) {

				//update position tracking
				mPreviousTouchPosition = mCurrentTouchPosition;
				mCurrentTouchPosition = mTouchMap[mObjectTouchIDs.front()];
				mOffsetUpdateAmount = mCurrentTouchPosition - mPreviousTouchPosition;
			}
		}

	void PanZoomView::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){

		//before removing the touch, if the total touches are less than 2 then ease the image to a stop

		removeTouch(touchID);
			if (mTouchMap.size() == 2){//Scaling
				mCurrentTouchPosition = mTouchMap[mObjectTouchIDs.front()];
				mInitalTouchDistance = 0.0f;

			}else{
					
				mMomentumUpdateAmount = (mCurrentTouchPosition - mPreviousTouchPosition) / 2;
				timeline().apply(&mMomentumUpdateAmount, Vec2f::zero(), mDecelerationRate, EaseOutQuad()).updateFn(std::bind(&PanZoomView::momentumUpdateFn, this));;
				mCurrentTouchPosition = Vec2f::zero();
			}
	}
	void PanZoomView::addTouch(int touchID, cinder::Vec2f touchPnt){
		mObjectTouchIDs.push_back(touchID);
		mTouchMap.insert(make_pair(touchID, touchPnt));
	}

	void PanZoomView::removeTouch(int touchID){
		std::vector<int>::iterator it;
		it = find(mObjectTouchIDs.begin(), mObjectTouchIDs.end(), touchID);
		if (it != mObjectTouchIDs.end()){
			mObjectTouchIDs.erase(it);
			mTouchMap.erase(touchID);
		}
	}


	const Vec2f PanZoomView::getTouchesMidpoint(){


		if (mTouchMap.size() == 2){
			Vec2f firstPoint = mTouchMap[mObjectTouchIDs.front()];
			Vec2f secondPoint = mTouchMap[mObjectTouchIDs.back()];
			return  firstPoint.lerp(0.5f, secondPoint);
		}
		else return Vec2f::zero();
	}

	const float PanZoomView::getTouchesDistance(){
		Vec2f firstPoint = mTouchMap[mObjectTouchIDs.front()];
		Vec2f secondPoint = mTouchMap[mObjectTouchIDs.back()];

		if (mTouchMap.size() == 2) return   firstPoint.distance(secondPoint);
		else return 0.0f;
	}



	void PanZoomView::update(){
		
	
			updateObjectPosition(mOffsetUpdateAmount);
			mOffsetUpdateAmount = Vec2f::zero();
		

			updateObjectScale(mScaleUpdateAmount);
			mScaleUpdateAmount = 0.0f;


			console() << "Object position " << mPanZoomObject->getPosition() << "  -- Scale " << mPanZoomObject->getScale().x << endl;



		
	
		 render();
	 }

	 void		PanZoomView::updateObjectPosition(ci::Vec2f posUpdateAmt){
		//Make sure the update keeps the object in bounds
		 Rectf objectRect = mPanZoomObject->getRect(GLOBAL);
		 Rectf viewRect = getRect(GLOBAL);
		 Rectf edgeDistanceRect = objectRect - viewRect;
				 
		 //Handle Horizontal constraints
		 float objectWidth  = mPanZoomObject->getWidth()  * mPanZoomObject->getScale().x;
	
		 bool objectIsGoingLeft = posUpdateAmt.x < 0;
		 bool objectIsWidthLargerThanView = (objectWidth > getWidth());

		 float leftDistance  = edgeDistanceRect.x1;
		 float rightDistance = edgeDistanceRect.x2;

		 float targetHorizontalDistance = (objectIsGoingLeft ^ objectIsWidthLargerThanView) ? leftDistance : rightDistance;
		 if (abs(posUpdateAmt.x) > abs(targetHorizontalDistance)) posUpdateAmt.x = -targetHorizontalDistance;



		 //Handle Vertical Constraints

		 float objectHeight = mPanZoomObject->getHeight() * mPanZoomObject->getScale().y;
		 bool objectIsGoingUp = posUpdateAmt.y < 0;
		 bool objectIsHeightLargerThanView = (objectHeight > getHeight());
		 float topDistance		= edgeDistanceRect.y1;
		 float bottomDistance	= edgeDistanceRect.y2;
		
		 
		 float targetVerticalDistance = (objectIsGoingUp ^ objectIsHeightLargerThanView) ? topDistance : bottomDistance;
		 if (abs(posUpdateAmt.y) > abs(targetVerticalDistance)) posUpdateAmt.y = -targetVerticalDistance;

			 mPanZoomObject->updatePosition(posUpdateAmt);
	 }


	 void		PanZoomView::updateObjectScale(float scaleUpdateAmt){

			 Vec2f updatedScale = mPanZoomObject->getScale() + Vec2f(scaleUpdateAmt, scaleUpdateAmt);
		
			 if (  updatedScale.x > mPanZoomObject->getMinScale().x  && updatedScale.x < mPanZoomObject->getMaxScale().x){
				 mPanZoomObject->updateScale(scaleUpdateAmt, getTouchesMidpoint()-getPosition() );
			
			
				 Vec2f scalePositionOffset = -(mPanZoomObject->getSize()* scaleUpdateAmt / 2.0f);
					 updateObjectPosition(scalePositionOffset);
			 }

		 }

	 void PanZoomView::render(){
		 Area area = gl::getViewport();
		 gl::SaveFramebufferBinding bindingSaver;
		 gl::setMatricesWindow(getWindowSize(), false);
		 // bind the framebuffer - now everything we draw will go there
		 mFbo.bindFramebuffer(); {


			 // clear out the FBO 
			 gl::clear(Color(0.0, 0.0, 0.0));
			 
			 mPanZoomObject->draw();


		 }
		 gl::setMatricesWindow(getWindowSize(), true);
	 }
	


	void PanZoomView::draw(cinder::Vec2f translationOffset) {
		//draw the main object, this is always drawing
		//Draw ScrollView Background
		gl::pushMatrices(); {
			gl::translate(translationOffset);
			
			gl::color(1, 1, 1);
			gl::draw(mFbo.getTexture(), getPosition());

			gl::enableAlphaBlending();
			gl::color(ColorA(1, 0, 0, .5));
			//mPanZoomObject->draw(getPosition());
			drawDebugBox();
		}gl::popMatrices();
	}
};