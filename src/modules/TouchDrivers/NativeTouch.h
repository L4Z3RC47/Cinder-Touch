//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: Code for native touch to be used with the touch manager class
//  Comments: This driver was based off of Cinder's multitouchbasic sample 
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/AppBasic.h"
#include "../../core/TouchManager.h"

#include "cinder/System.h"

class NativeTouch{
public:
	NativeTouch();
	~NativeTouch();
	void connect();
	void disconnect();

private:
	//Touch Event Callbacks
    boost::signals2::scoped_connection  mTouchBeganCallBack,
										mTouchMovedCallBack,
										mTouchEndCallBack;
	
   void nativeTouchBegan(const cinder::app::TouchEvent &event);
   void nativeTouchMoved(const cinder::app::TouchEvent &event);
   void nativeTouchEnded(const cinder::app::TouchEvent &event);
   
   std::shared_ptr<class TouchManager>	mTouchManagerPntr;
   int									mTouchId;
};