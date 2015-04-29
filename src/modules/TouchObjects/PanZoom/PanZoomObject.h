//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2015
//	Developers: Stacey Martens & Paul Rudolph
//  Contents: 
//  Comments:
/*
 Instructions for laying out hotspots :
 1. Open the image at 100% in photoshop.
 2. Create a rectangle the size of your mImageBoundary, 50% opacity.
 3. Highlight the are you want to show with the premade rectangle.
 4. Get the center coordinates of where the rectangle is now sitting -- this is your hotspot point.
 5. If you want to show more in that area, set the zoom to a lower number after you get the coordinate.
 We don't want to go higher than 100% (aka Vec2f(1, 1) because we don't distort the image.
 */

//----------------------------------------------------------------------------
#pragma once
#include "cinder/app/AppNative.h"

//touches
#include "BaseTouchObject.h"
#include <list>
#include <mutex>

#include "cinder/gl/Fbo.h"

namespace touchObject{

	//References
	typedef std::shared_ptr<class		PanZoomObject>		PanZoomObjectRef;
	typedef std::shared_ptr<const class PanZoomObject>		PanZoomObjectConstRef;
	typedef std::weak_ptr  <class		PanZoomObject>      PanZoomObjectWeakRef;


class PanZoomObject : public touchObject::BaseTouchObject{
public:

	PanZoomObject();
	static  PanZoomObjectRef createZoomableImage(const cinder::gl::TextureRef &image, float minScale, float maxScale);

    
	//Update
	void  updatePosition(ci::Vec2f positionOffsetAmt);
	void  updateScale(float scaleOffsetAmt, const ci::Vec2f &scalePoint);


	void   setMinScale(ci::Vec2f scale){ mMinScale = scale; };
	ci::Vec2f getMinScale(){ return mMinScale; };
	void   setMaxScale(ci::Vec2f scale){ mMaxScale = scale; };
	ci::Vec2f getMaxScale(){ return mMaxScale; };
	//Image Handling
	void			   setImage(const cinder::gl::TextureRef &image);
	ci::gl::TextureRef getImage(){ return mImage; };

	void			setImageBounds(const ci::Rectf &bounds){ mImageBounds = bounds; };
	cinder::Rectf	getImageBoundary(){ return mImageBounds; };
    
	void			removeTouchFromTouchMap(int touchID);

    cinder::Vec2f   getImageScaleUsingFingerDistance();
	
    
    float           getDurationOfChange(const float &newScaleX, const cinder::Vec2f &pointLocalToImage, const cinder::Vec2f &movePointTo);
    
    cinder::Vec2f	keepImageWithinBounds(cinder::Vec2f newPos);
    cinder::Vec2f   keepScaleWithinBounds(cinder::Vec2f newScale);
    
    float           getDistanceBetweenPoints( const cinder::Vec2f &pnt1, const cinder::Vec2f &pnt2 );
    
    bool            isWidthProportionGreaterThanHeight();
    void            settleImageWithEasing();
    const cinder::Vec2f   getCenterOfTwoFingers();
    
    void            prepareToZoomAndMove(const cinder::Vec2f &pointLocalToImage,  const cinder::Vec2f &movePointTo, cinder::Vec2f scaleTo, bool zoomingToHotspot, bool zoomingFromPinch, bool zoomOutAndIn);
    void            performZoom(const cinder::Vec2f &pointLocalToImage, const cinder::Vec2f &movePointTo, cinder::Vec2f scaleTo, float duration, bool zoomingToHotspot,
    bool zoomingFromPinch, const std::function<float (float)> &fn);
    
    cinder::Vec2f   getCenterOfImageView()  {return mCenterOfImageView;};
    cinder::Vec2f   getLocalTouchPnt( const cinder::Vec2f &globalPnt );
    float           getLerp(float start, float end, float percent);

    
    

    void             updateImagePosition(const cinder::Vec2f &newPos);

    void            drawImageToFBO();
    
	virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());
    
    void            resetImage();
    
    //used in params
    float                           mThrowMultiplier,
                                    mScaleMultiplier;
    
private:
        
    static std::shared_ptr<class Object>        mObjectInstance;
//    std::shared_ptr<class ViewFinderController> mViewFinderController;

    bool                            mPinching;
    std::pair<float, cinder::Vec2f> mInitialScaleReference;
    float                           mCurDistBetweenFingers;
    
    //keep track of object position and scale
	cinder::Vec2f					mOrigImageSize,
									mPrevPos,
									mCurPos,
                                    mPosDiff,
                                    mMinScale,
                                    mMaxScale,
									mScalePoint,
                                    mTrackingPoint,
                                    mTrackingPointLocalToImage,
                                    mCenterOfImageView;
    
  std::pair< int, cinder::Vec2f >	mTouchPnt1, mTouchPnt2;
  std::map<int, cinder::Vec2f>		mTouchMap;
    
  //Image Properties
	cinder::gl::TextureRef			mImage;
	cinder::Rectf					mImageBounds;
    

    
	cinder::Anim<cinder::Vec2f>		mPosAnim,
                                    mScaleAnim;
    
    //used when we are zooming to a point to do update function calls during
    cinder::Anim<float>             mTimer;
    
	};
};