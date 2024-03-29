//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2015
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------
#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

//touches
#include "BaseTouchObject.h"

class BlueObj : public touchObject::BaseTouchObject{
public:
	BlueObj();
	
	void setup();
	virtual void	draw(ci::vec2 parentTranslatePos = ci::vec2(0));

	//use virtual to override the BaseTouchObject calls
	//with these functions, we will drag the objects around on the screen
	virtual	void	touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	
private:
	ci::vec2				mCurPos, mPrevPos;
	float					mRadius;
};