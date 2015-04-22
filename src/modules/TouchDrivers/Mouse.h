//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: Code for mouse driver to be used with the touch manager class
//  Comments: This mouse driver was based off of Cinder's listenerBasic sample 
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/AppBasic.h"
#include "../../core/TouchManager.h"

class Mouse{
public:
	Mouse();
	~Mouse();
	void connect(bool debugMode = false);
	void disconnect();

private:
	 //Mouse Event Callbacks
    boost::signals2::scoped_connection  mMouseBeganCallBack,
                                        mMouseMovedCallBack,
                                        mMouseEndCallBack,
										mKeyboardKeyDownCallBack,
										mKeyboardKeyUpCallBack;

   
   void mouseBegan(const cinder::app::MouseEvent &event);
   void mouseMoved(const cinder::app::MouseEvent &event);
   void mouseEnded(const cinder::app::MouseEvent &event);
   
   //DEBUG
   void keyDown(const cinder::app::KeyEvent &event);
   void keyUp(const cinder::app::KeyEvent &event);

   std::shared_ptr<class TouchManager>	mTouchManagerPntr;
   int									mTouchId;
   
   bool mDebugMode,mCrtlPressed, mIsAcceptingCntrPoint;
   ci::Vec2f mDebugCenterPoint;
};