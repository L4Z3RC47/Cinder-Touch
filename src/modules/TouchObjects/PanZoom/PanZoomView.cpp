#include "PanZoomView.h"

#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace touchObject{

	PanZoomView::PanZoomView() :
		mCurrentTouchDistance(0.0f),
		mPositionUpdateAmount(Vec2f::zero()),
		mScaleUpdateAmount(0.0f)
	{

	}

	PanZoomView::~PanZoomView(){
	}

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



	void PanZoomView::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
	

		//Only accept 2 Touches
		if (mTouchMap.size() < 2){


			addTouch(touchID, touchPnt);

			//update touch point tracking variables - if the first one is not taken, create touch point 1, otherwise create touch point 2
			if (mTouchMap.size() == 1){
				mCurrentTouchPosition = touchPnt;
			
			}else if (mTouchMap.size() == 2){

				mCurrentTouchPosition = getTouchesMidpoint();
				mCurrentTouchDistance = getTouchesDistance();
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
			mScaleUpdateAmount = ((mCurrentTouchDistance - mPreviousTouchDistance) / mPreviousTouchDistance) ;
		
		}
			//not pinching, just one finger moving things around
		else if (mTouchMap.size() == 1 ) {

				//update position tracking
				Vec2f previousTouchPosition = mCurrentTouchPosition;
				mCurrentTouchPosition = mTouchMap[mObjectTouchIDs.front()];
				mPositionUpdateAmount = mCurrentTouchPosition - previousTouchPosition;
			}
		}

	void PanZoomView::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){

		//before removing the touch, if the total touches are less than 2 then ease the image to a stop

		removeTouch(touchID);
			if (mTouchMap.size() == 2){//Scaling
				mCurrentTouchPosition = mTouchMap[mObjectTouchIDs.front()];
			}else{
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
		if (abs(mPositionUpdateAmount.value().x) > 0.00025 || abs(mPositionUpdateAmount.value().y) > 0.00025){
			updateObjectPosition(mPositionUpdateAmount);
		}
		
		if (abs(mScaleUpdateAmount) > 0.00025){
			updateObjectScale(mScaleUpdateAmount);
		}
		
		 timeline().apply(&mPositionUpdateAmount, Vec2f::zero(), 0.50, EaseOutExpo());
		 timeline().apply(&mScaleUpdateAmount, 0.0f, 1.0, EaseOutExpo());
		 console() << "POSITION UPDATE AMT " << mPositionUpdateAmount << endl;
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
			 console() << "UPDATED SCALE : " << updatedScale << endl;
			 if (  updatedScale.x > mPanZoomObject->getMinScale().x  && updatedScale.x < mPanZoomObject->getMaxScale().x){
				 mPanZoomObject->updateScale(scaleUpdateAmt, getTouchesMidpoint()-getPosition() );
			
			
				 //Vec2f scalePositionOffset = -(mPanZoomObject->getSize()* scaleUpdateAmt / 2.0f) - getTouchesMidpoint();
					// updateObjectPosition(scalePositionOffset);
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
	/*
	void PanZoomView::zoomToPoint(const int &hotspotID, const Vec2f &hotspotCoordinate, const Vec2f &hotspotScale, bool zoomOutAndIn){

		//the old point =
		//the new point =
		//so the distance between should be used to determine the duration of time it takes to make that move

		//tell the object to move to the corrent spot
		//the distance traveled in the zoom to point will tell the overlay when to show
		//if(mMainObject)             mMainObject->prepareToZoomAndMove(hotspotCoordinate, mMainObject->getCenterOfImageView(), hotspotScale, true, false, zoomOutAndIn);
		//hide the hotspots
		//if(mHotspotController)      mHotspotController->hideHotspots(hotspotID);
		//show correct overlay
		// if(mOverlay)                mOverlay->addActiveOverlay(hotspotID);

		//only the first time a hotspot is touched
		//if(mFirstHotspotTouch){
		//    mFirstHotspotTouch = false;
		//    mViewFinderController->firstHotspotTouched();
		// }
	}
	*/

	/*
	void PanZoomView::resetImage(){

		//if(mMainObject)             mMainObject->resetImage();

	}
	*/
	void PanZoomView::draw(cinder::Vec2f translationOffset) {
		//draw the main object, this is always drawing
		//Draw ScrollView Background
		gl::pushMatrices(); {
			gl::translate(translationOffset);
			
			
			gl::color(1, 1, 1);
			//gl::draw(mFbo.getTexture(), getPosition());

			gl::enableAlphaBlending();
			gl::color(ColorA(1, 0, 0, .5));
			mPanZoomObject->draw(getPosition());
			drawDebugBox();
		}gl::popMatrices();
	}
};