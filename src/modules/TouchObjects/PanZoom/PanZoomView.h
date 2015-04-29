//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2015
//	Developers: 
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------
#pragma once
#include "cinder/gl/Fbo.h"
#include "BaseTouchObject.h"
#include "PanZoomObject.h"

namespace touchObject {

	typedef std::shared_ptr<class		PanZoomView>		PanZoomViewRef;
	typedef std::shared_ptr<const class PanZoomView>		PanZoomViewConstRef;
	typedef std::weak_ptr  <class		PanZoomView>		PanZoomViewWeakRef;

	class PanZoomView : public touchObject::BaseTouchObject{
	public:
		PanZoomView();
		~PanZoomView();


		static PanZoomViewRef create(cinder::Vec2f pos, cinder::Vec2f size);
		void	addObject(PanZoomObjectRef pzObject){ 
			mPanZoomObject = pzObject;
			mPanZoomObject->setParentPosition(getPosition());
		};

		virtual void update();
		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());


		void    zoomToPoint(const int &hotspotID, const cinder::Vec2f &hotspotCoordinate, const cinder::Vec2f &hotspotScale, bool zoomOutAndIn);

		void    mainImageWasTouched();

		void    resetImage();


		const ci::Vec2f getTouchesMidpoint();
		const float getTouchesDistance();
	protected:
		//use virtual to override the BaseTouchObject calls
		//with these functions, we will drag the objects around on the screen
		virtual	void	touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		virtual void	touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		virtual void	touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		
		void			updateObjectPosition(ci::Vec2f posUpdateAmt);
		void			updateObjectScale(float scaleUpdateAmt);
		void			render();
		void			addTouch(int touchID, cinder::Vec2f touchPnt);
		void			removeTouch(int touchID);

	private:

		//Touch Handling
		std::map<int, cinder::Vec2f>	mTouchMap;
		cinder::Vec2f					mCurrentTouchPosition;
		float							mCurrentTouchDistance;


		cinder::Anim<ci::Vec2f> mPositionUpdateAmount;
		cinder::Anim<float>		mScaleUpdateAmount;

		PanZoomObjectRef mPanZoomObject;
		//Used for Masking the view 
		cinder::gl::Fbo				mFbo;

	};
};