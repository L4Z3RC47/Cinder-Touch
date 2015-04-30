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

		//Creation Functions
		static PanZoomViewRef create(cinder::Vec2f pos, cinder::Vec2f size);
		void			addObject(PanZoomObjectRef pzObject);


		
		//External Interaction
		/**
		*	Programatically shifts the content to a new origin
		*/
		void			setContentOffset(ci::Vec2f pnt, bool animated=true);
		/**
		*	Programatically adjusts Zoom 
		*/
		void			zoomToScale(float scale, bool animated=true);
		/**
		*	Programatically adjusts orign and zoom level
		*	@Param "zoomOutAndIn" will animate the zoom level out first, then to the new zoom level while panning to the new origin point.
		*/
		void			zoomToPoint(const cinder::Vec2f &point, const cinder::Vec2f &scale, bool zoomOutAndIn=true);
		
		/**
		*	Resets content's origin and zoom to original location.
		*/
		void			resetContent();

		virtual void	update();
		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());

		void momentumUpdateFn();

	protected:
		//use virtual to override the BaseTouchObject calls
		//with these functions, we will drag the objects around on the screen
		virtual	void	touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		virtual void	touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		virtual void	touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		
		void			addTouch(int touchID, cinder::Vec2f touchPnt);
		void			removeTouch(int touchID);
		
		const ci::Vec2f getTouchesMidpoint();
		const float		getTouchesDistance();

		void			updateObjectPosition(ci::Vec2f posUpdateAmt);
		void			updateObjectScale(float scaleUpdateAmt);
		void			render();
	
	

	private:

		//Touch Handling
		std::map<int, cinder::Vec2f>	mTouchMap;
		
		cinder::Vec2f					mPreviousTouchPosition,
										mCurrentTouchPosition;
		
		double							mTouchTimeStamp;
		
		float							mInitalTouchDistance,
										mCurrentTouchDistance;
	

		
		cinder::Anim<ci::Vec2f>			mOffsetUpdateAmount,mMomentumUpdateAmount;
		cinder::Anim<float>				mScaleUpdateAmount;
		
		ci::Rectf						mContentInsets;

		bool							mScrollBounceEnabled,mZoomBouceEnabled, mTracking,mZooming;
		
		float							mDecelerationRate;

		

		PanZoomObjectRef				mPanZoomObject;
		
		//Used for Masking the view 
		cinder::gl::Fbo					mFbo;

	};
};