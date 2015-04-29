
#include "PanZoomObject.h"

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Timeline.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

namespace touchObject{
	PanZoomObject::PanZoomObject() : BaseTouchObject(),
		mPinching(false),
		mPrevPos(Vec2f::zero()),
		mCurPos(Vec2f::zero()),
		mPosDiff(Vec2f::zero()),
		mMinScale(Vec2f(1.0f, 1.0f)),               //default, this is set based on proportions in the setup
		mMaxScale(Vec2f(1.0f, 1.0f)),               //default, we don't want it to be larger than the original file due to loss in quality
		mScalePoint(Vec2f::zero()),
		mTrackingPoint(Vec2f::zero()),
		mTrackingPointLocalToImage(Vec2f::zero()),
		mCenterOfImageView(Vec2f::zero()),
		mCurDistBetweenFingers(0),
		mImageBounds(0.0f, 0.0f, 100.0f, 100.0f), //default, this is set in the setup
		mPosAnim(Vec2f::zero()),
		mScaleAnim(Vec2f::zero()),
		mScaleMultiplier(.03f),
		mThrowMultiplier(9.0f),
		mTimer(0.0f)
	{

	}

	PanZoomObjectRef	PanZoomObject::createZoomableImage(const cinder::gl::TextureRef &image, float minScale, float maxScale){
		PanZoomObjectRef objectRef(new PanZoomObject());
		objectRef->setImage(image);
		objectRef->setMinScale(ci::Vec2f(minScale, minScale));
		objectRef->setMaxScale(ci::Vec2f(maxScale, maxScale));
		
		objectRef->registerWithTouchMngr();
		objectRef->setAcceptTouch(true);
		//SET THE BOUNDARIES FOR THE VIEWABLE SPACE OF THE IMAGE
		//objectRef-> mImageBounds = Rectf(ci::Vec2f::zero(), objectRef->getImage()->getSize());

	
	
		return objectRef;
	}
	//Setup Functionality

	//Update Functionality
	void PanZoomObject::updatePosition(Vec2f positionOffsetAmt){
		//check bounds, Keep the object in view
		
		
		//check if the left most sections position  is past the  left break point
		//float dist = (mBreakLineFront - cellFrontEdge);
		//if (offsetAmt > dist) offsetAmt = dist;



		setPosition(getPosition() + positionOffsetAmt);
	
	}
	
	void  PanZoomObject::updateScale(float scaleOffsetAmt, const ci::Vec2f &scalePoint){
	
				setScale(getScale() + Vec2f(scaleOffsetAmt, scaleOffsetAmt));
			mScalePoint = scalePoint;
		//	console() << "SCALE CENTER POINT " << scalePoint << "  Position :" << getPosition() << endl;
		
	}


	/*
	void PanZoomObject::setup(const cinder::gl::TextureRef &image) {
		mImage = image;
		mOrigImageSize = Vec2f(mImage->getWidth(), mImage->getHeight());

		//    mViewFinderController = ViewFinderController::getInstance();

		//SET THE BOUNDARIES FOR THE VIEWABLE SPACE OF THE IMAGE
		mImageBoundary = Rectf(654.0f, 0.0f, 1920.0f, 1080.0f);

		//touch settings
		setSize(mImage->getSize());
		registerWithTouchMngr();
		setAcceptTouch(true);

		//set min scale based on proportions of viewing area. Max scale is hand coded to 1, so the image doesn't look cruddy.
		mMinScale = setMinScale();

		//set the original position to the minimums so the full image is in view
		setScale(mMinScale);
		setPosition(keepImageWithinBounds(Vec2f(0, 0)));

		//set these so they don't jump the first time things move
		mScaleAnim = getScale();
		mPosAnim = getPosition();

		//initiate fake touch points to start
		mTouchPnt1 = make_pair(-2, Vec2f::zero());
		mTouchPnt2 = make_pair(-2, Vec2f::zero());

		//mCenterOfImageView is the place we want all hotspots to slide to when they are selected
		mCenterOfImageView = Vec2f(mImageBoundary.getCenter());

		//start in the center
		performZoom(Vec2f(mOrigImageSize.x / 2, mOrigImageSize.y / 2), mCenterOfImageView, mMinScale, 1.1f, true, false, EaseNone());
	}
	*/

	void		PanZoomObject::setImage(const cinder::gl::TextureRef &image){
		mImage = image; 
		setSize(mImage->getSize());
	}
	/*
	Vec2f PanZoomObject::setMinScale(){

		float minScale = 0.0f; //set default
		float forcedAmountSmaller = 0.01f; //increasing this number increases the amount of empty space around the image when it's at it's smallest size

		if (isWidthProportionGreaterThanHeight())   minScale = mImageBounds.getHeight() / mImage->getHeight() - forcedAmountSmaller;
		else                                        minScale = mImageBounds.getWidth() / mImage->getWidth() - forcedAmountSmaller;

		return Vec2f(minScale, minScale);
	}
	*/
	bool PanZoomObject::isWidthProportionGreaterThanHeight(){

		float widthProportion = mImageBounds.getWidth() / mImage->getWidth();
		float heightProportion = mImageBounds.getHeight() / mImage->getHeight();
		return (widthProportion > heightProportion);
	}
	/*
	void PanZoomObject::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
	//	 console() << "Object::touchesBeganHandler" << endl;

		//add the touch, we're only accepting 2
		if (mTouchMap.size() < 2){

			//stop any throw animations that could be happening
			timeline().removeTarget(mPosAnim.ptr());
			timeline().removeTarget(mScaleAnim.ptr());

			//update touch point tracking variables - if the first one is not taken, create touch point 1, otherwise create touch point 2
			if (mTouchMap.size() == 0){
				mTouchPnt1.first = touchID;
				mTouchPnt1.second = touchPnt;
				mTrackingPoint = mCurPos = touchPnt;

			}else if (mTouchMap.size() == 1){
			
				mTouchPnt2.first = touchID;
				mTouchPnt2.second = touchPnt;
				mTrackingPoint = getCenterOfTwoFingers();
				//Caputure the current scale
				Vec2f curScale = getScale();
				mInitialScaleReference = std::make_pair(getDistanceBetweenPoints(mTouchPnt1.second, mTouchPnt2.second), curScale);
				
				//Capture the initial distance between the 2 fingers, so we can track how far apart they spread for scale
				mCurDistBetweenFingers = getDistanceBetweenPoints(mTouchPnt1.second, mTouchPnt2.second);

			
				mTrackingPointLocalToImage = getLocalTouchPnt(mTrackingPoint);

				mPinching = true;

			}

			
			addTouchToTouchMap(touchID, touchPnt);


		}

	}
	*/
	/*
	void PanZoomObject::addTouchToTouchMap(int touchID, Vec2f touchPnt){
		mTouchMap.insert(make_pair(touchID, touchPnt));
	}
	*/
	/*
	void PanZoomObject::touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
		//    console() << "Object::touchesMovedHandler" << endl;

		//keep track of the 2 touch points, update the related position here
		if (mTouchPnt1.first == touchID) mTouchPnt1.second = touchPnt;
		else if (mTouchPnt2.first == touchID) mTouchPnt2.second = touchPnt;
		else                                    touchesBeganHandler(touchID, touchPnt, touchType);


		//if the touchpoint is within the bounds
		//if (mImageBounds.contains(touchPnt)){
			if (mPinching){
				//now we just focus on scale changes
				Vec2f newScale = getImageScaleUsingFingerDistance();
				if (newScale != Vec2f::zero()){
					prepareToZoomAndMove(mTrackingPointLocalToImage, mTrackingPoint, newScale, false, true, false);
				}
			//}
			//not pinching, just one finger moving things around
			else if (mTouchMap.size() < 2 && (mTouchPnt1.first == touchID || mTouchPnt2.first == touchID)) {

				Vec2f imagePos = getPosition();

				//based on touch ID's, determine which finger we should be following
				//inactive fingers are -2, so any active finger will have a higher ID
				mTrackingPoint = mTouchPnt1.first > mTouchPnt2.first ? mTouchPnt1.second : mTouchPnt2.second;

				//update position tracking
				mPrevPos = mCurPos;
				mCurPos = mTrackingPoint;

				//get the offset
				mPosDiff = (mCurPos - mPrevPos);
				imagePos += mPosDiff*1.5;

				//update image position, which updates viewfinder reflection
				imagePos = keepImageWithinBounds(imagePos);

				//duration is based on distance it has to move so it always feels the same speed
				float duration = getDistanceBetweenPoints(imagePos, mPosAnim) / 1200;

				timeline().apply(&mPosAnim, imagePos, duration, EaseOutQuad())
					.updateFn([=](){
					updateImagePosition(mPosAnim);
				});

				//double check
				setAcceptTouch(true);
			}
		}
	}
	*/
	cinder::Vec2f PanZoomObject::getLocalTouchPnt(const Vec2f &globalPnt){
		//    console() << "Object::getLocalTouchPnt" << endl;
		//turn the global coordinate into the local coordinate relative to this object
		return (globalPnt - getPosition()) / mScale;
	}

	void PanZoomObject::resetImage(){
		//    console() << "Object::resetImage" << endl;

		//kill all tweens and delays
		timeline().removeTarget(mPosAnim.ptr());
		timeline().removeTarget(mScaleAnim.ptr());
		timeline().removeTarget(mTimer.ptr());

		performZoom(Vec2f(mOrigImageSize.x / 2, mOrigImageSize.y / 2), mCenterOfImageView, mMinScale, 1.1f, true, false, EaseOutQuad());
	}

	//void Object::zoomToPoint(const Vec2f &point, Vec2f scale, const Vec2f &movePointTo, float duration, bool zoomingToHotspot, bool zoomingFromPinch){

	//point = the point local to the image that we will move
	//scale = the new scale
	//movePointTo = where the point is going
	void PanZoomObject::prepareToZoomAndMove(const Vec2f &pointLocalToImage, const Vec2f &movePointTo, Vec2f scaleTo, bool zoomingToHotspot, bool zoomingFromPinch, bool zoomOutAndIn){

		timeline().removeTarget(mTimer.ptr());

		setAcceptTouch(false);

		//if you're zooming to a hotspot, first zoom out a little bit, then zoom in to what we want
		if (zoomingToHotspot){

			float dur = 0.0f;

			if (zoomOutAndIn){

				//first zoom out a little bit
				//the smaller scale is the image as it zooms out (before back in)
				Vec2f smallerScale = keepScaleWithinBounds(getScale() - Vec2f(.4f, .4f));
				Vec2f pt1 = getLocalTouchPnt(movePointTo);  //the current center of the screen, local to the image
				Vec2f pt2 = pointLocalToImage;              //the point on the image, local to the image
				Vec2f halfwayPnt = pt1.lerp(0.5f, pt2);//halfway to the final destination;

				float length = getDistanceBetweenPoints(pt1, halfwayPnt);

				if (smallerScale.x < getScale().x && smallerScale.x < scaleTo.x){
					Vec2f currentCenterShowingOnScreen = getLocalTouchPnt(mCenterOfImageView);
					Vec2f directionToPoint = currentCenterShowingOnScreen - pointLocalToImage;
					directionToPoint.safeNormalize();
					Vec2f pointAlongTheWayLocalToImage = currentCenterShowingOnScreen + (-directionToPoint * length);

					dur = getDurationOfChange(smallerScale.x, pointAlongTheWayLocalToImage, movePointTo);
					performZoom(pointAlongTheWayLocalToImage, movePointTo, smallerScale, dur, false, true, EaseInSine());
				}
			}

			//then zoom in to where we want it
			timeline().apply(&mTimer, 1.0f, (float)(dur - 0.05f))
				.finishFn([=](){
				float dur = getDurationOfChange(scaleTo.x, pointLocalToImage, movePointTo);
				performZoom(pointLocalToImage, movePointTo, scaleTo, dur, zoomingToHotspot, zoomingFromPinch, EaseOutSine());
			});
		}
		//IF WE'RE PINCHING, the distance is changing based on the finger movements, not on the movement of the image
		else{
			float dur = 0.1f;

	
			performZoom(pointLocalToImage, movePointTo, scaleTo, dur, zoomingToHotspot, zoomingFromPinch, EaseNone());
		}
	}

	float PanZoomObject::getDurationOfChange(const float &newScaleX, const Vec2f &pointLocalToImage, const cinder::Vec2f &movePointTo){
		Vec2f pt1 = getLocalTouchPnt(movePointTo);  //the current center of the screen, local to the image
		Vec2f pt2 = pointLocalToImage;              //the point on the image, local to the image

		float scaleChange = fabs(getScale().x - newScaleX); //positive float that represents the difference in scale
		float distanceChange = getDistanceBetweenPoints(pt1, pt2);
		float dur = distanceChange / 1600 + scaleChange * 2;

		//console() << "   " << endl;
		//console() << "pt1  = "  << pt1 << "  pt2 = " << pt2 << endl;
		//console() << "dur = " << dur << "   distanceChange = " << distanceChange << endl;

		if (dur < 1.5f) dur = 1.5f;
		return dur;
	}

	//ZOOM ACTION
	void PanZoomObject::performZoom(const Vec2f &pointLocalToImage, const Vec2f &movePointTo, Vec2f scaleTo, float duration, bool zoomingToHotspot, bool zoomingFromPinch, const std::function<float(float)> &fn){
		console() << "ZOOM " << scaleTo << endl;
		//keep the scale within bounds
		scaleTo = keepScaleWithinBounds(scaleTo);
		console() << "INBOUNDS ZOOM " << scaleTo << endl;
		//during zoom, we don't want to compete with the bounds so we'll let it go wherever and then settle it when we are done
		//We'll determine the new position for the image by converting the point that is local to the image (the one we want to move to the center)
		Vec2f newPos;
		if (zoomingFromPinch || zoomingToHotspot) newPos = movePointTo - pointLocalToImage*scaleTo.x;
		else                                     newPos = keepImageWithinBounds(movePointTo - pointLocalToImage*scaleTo.x);

		//get current scale, so we can animate smoothly from here
		mScaleAnim = getScale();
		//animate the scale
		timeline().apply(&mScaleAnim, scaleTo, duration, fn)
			.updateFn([=](){
			mScale = mScaleAnim;
			setScale(mScale);

			//        console() << "in the perform zoom --- mScale = " << mScale << endl;
		});

		//get cur position so we can animate smoothly from here
		mPosAnim = getPosition();
		//animate the position
		timeline().apply(&mPosAnim, newPos, duration, fn)
			.updateFn([=](){
			updateImagePosition(mPosAnim);
		})
			.finishFn([=](){
			setAcceptTouch(true);

			//little ease on the zooming
			if (zoomingFromPinch){
				//if we were pinching, then use this time to settle the image
				Vec2f curPos = keepImageWithinBounds(getPosition());
				timeline().apply(&mPosAnim, curPos, 0.1f, EaseOutSine())
					.updateFn([=](){
					updateImagePosition(mPosAnim);
				});
			}

		});

		//show the active overlay, if we're going to a hotspot
		if (zoomingToHotspot){
			// Overlay::getInstance()->animateOn(duration - 2.0f);
			//hide the active hotspot, at the same time as the overlay is being shown
			// if(!zoomingFromPinch) ObjectHotspotController::getInstance()->hideActiveHotspot(duration - 3.0f);
		}

	}

	//get halfway between the 2 fingers
	const Vec2f PanZoomObject::getCenterOfTwoFingers() {
		return mTouchPnt1.second.lerp(0.5f, mTouchPnt2.second);
	}

	void PanZoomObject::updateImagePosition(const cinder::Vec2f &newPos){
		setPosition(newPos);
	}

	Vec2f PanZoomObject::getImageScaleUsingFingerDistance(){
		//the current scale of the image
		Vec2f origScale = getScale();
		//determine the difference in size
		float prevDistBetweenFingers = mCurDistBetweenFingers;
		mCurDistBetweenFingers = getDistanceBetweenPoints(mTouchPnt1.second, mTouchPnt2.second);
		float distanceDiff = mCurDistBetweenFingers - prevDistBetweenFingers;

		float scaleDiff = distanceDiff*mScaleMultiplier;
		//    console() << "scaleDiff = " << scaleDiff << endl;
		Vec2f newScale = origScale + Vec2f(scaleDiff, scaleDiff);
		return newScale;
	}

	float PanZoomObject::getDistanceBetweenPoints(const cinder::Vec2f &pnt1, const cinder::Vec2f &pnt2){
		return pnt1.distance(pnt2);
	}

	cinder::Vec2f PanZoomObject::keepImageWithinBounds(Vec2f newPos){
		//if the image is smaller than the bounds, then center it in the boundary area
		//if the image is larger than the bounds, let it be offset but keep the edges from going too far in one direction or the other
		//determine this separately for both the x & y positions
		Vec2f curImageSize = Vec2f(getWidth()*mScale.x, getHeight()*mScale.y);

		float offset = 400.0f; //the amount from the edges of the boundary the image can come
		float newLeft = mImageBounds.x1 + offset;
		float newRight = mImageBounds.x2 - mOrigImageSize.x*mScale.x - offset;
		float newBottom = mImageBounds.y2 - curImageSize.y - offset;
		float newTop = mImageBounds.y1 + offset;

		//horizontal constraints
		//if the x position is too far left, attach it to the right side
		if (newPos.x + curImageSize.x <= mImageBounds.x2 - offset)    newPos.x = newRight;
		//if the x position is too far right, attach it to the left side
		else if (newPos.x >= mImageBounds.x1 + offset)                newPos.x = newLeft;

		//if the image would be pulled too far up from the bottom of the screen
		if (newPos.y + curImageSize.y <= mImageBounds.y2 - offset)    newPos.y = newBottom;
		//or if the image would be pulled too far from the top of the screen
		else if (newPos.y >= mImageBounds.y1 + offset)                newPos.y = newTop;

		return newPos;
	}

	Vec2f PanZoomObject::keepScaleWithinBounds(Vec2f newScale){
		//    console() << "Object::keepScaleWithinBounds" << endl;

		if (newScale.x >= mMaxScale.x)	newScale.x = mMaxScale.x;
		else if (newScale.x <= mMinScale.x) newScale.x = mMinScale.x;

		if (newScale.y >= mMaxScale.y)	newScale.y = mMaxScale.y;
		else if (newScale.y <= mMinScale.y) newScale.y = mMinScale.y;

		return newScale;
	}
	/*
	void PanZoomObject::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
		//    console() << "Object::touchesEndedHandler" << endl;

		//before removing the touch, if the total touches are less than 2 then ease the image to a stop

		if (mTouchPnt1.first == touchID || mTouchPnt2.first == touchID){

			if (mPinching){
				//end all timeline that are zooming things
				setAcceptTouch(true);

				mTouchMap.clear();
				mTouchPnt1.first = -2;
				mTouchPnt2.first = -2;
				mTouchPnt1.second = Vec2f::zero();
				mTouchPnt2.second = Vec2f::zero();

				mTrackingPoint = Vec2f::zero();

			}
			else{
				//handle the touch map
				removeTouchFromTouchMap(touchID);

				//if another point still exists, need to reset the curPos to that finger to avoid jumps in the image
				if (mTouchPnt1.first == -2)       mTouchPnt1.second = Vec2f::zero();
				else if (mTouchPnt2.first == -2) mTouchPnt2.second = Vec2f::zero();

				settleImageWithEasing();
			}

			mPinching = false;
		}
	}
	*/
	void PanZoomObject::settleImageWithEasing(){
		//    console() << "Object::settleImageWithEasing" << endl;

		//POSITION
		Vec2f oldPos = getPosition();
		Vec2f newPos = mPosAnim = getPosition();
		newPos = keepImageWithinBounds(newPos + mPosDiff*mThrowMultiplier);

		float distance = getDistanceBetweenPoints(oldPos, newPos);

		//if you haven't really moved it that far, don't settle it -- this may need to be adjusted on the hardware to find a number that works
		if (distance < 60.0f) newPos = oldPos;

		float duration = distance / 100.0f;
		//duration should be based on distance needing to be traveled
		timeline().apply(&mPosAnim, (Vec2f)newPos, duration, EaseOutSine())
			.updateFn([=](){
			updateImagePosition(mPosAnim);
		});

		mPosDiff = Vec2f::zero();
	}

	void PanZoomObject::removeTouchFromTouchMap(int touchID) {
		//   console() << "Object::removeTouchFromTouchMap" << endl;

		//if either of the active fingers are picked up, take them out of the map
		for (map<int, Vec2f>::iterator iter = mTouchMap.begin(); iter != mTouchMap.end();) {
			if (iter->first == touchID){
				//set the touch id to -2, a number that won't be used
				if (mTouchPnt1.first == iter->first) mTouchPnt1.first = -2;
				else if (mTouchPnt2.first == iter->first) mTouchPnt2.first = -2;

				//remove id from touchmap
				iter = mTouchMap.erase(iter);
			}
			else ++iter;
		}
	}

	void PanZoomObject::draw(cinder::Vec2f translationOffset) {

		gl::pushMatrices(); {
			//translate position for graphics
			gl::translate(translationOffset);
		

			//draw the images
			//set the scale for drawing
			

			gl::color(1, 1, 1);
			gl::draw(mImage, getRect(LOCAL) );
			
			drawDebugBox();
		}gl::popMatrices();

	}
};