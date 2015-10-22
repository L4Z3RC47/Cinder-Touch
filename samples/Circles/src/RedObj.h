//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2015
//	Developers: Stacey Martens
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------
#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

//touches
#include "BaseTouchObject.h"

//more touchable objects
#include "BlueObj.h"

class RedObj : public touchObject::BaseTouchObject{
public:
	RedObj();
	
	void setup();
	virtual void	draw();

	//use virtual to override the BaseTouchObject calls
	//with these functions, we will drag the objects around on the screen
	virtual	void	touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	
private:
	std::shared_ptr<BlueObj> mBlueObj;

	//keep track of object position
	ci::vec2				mCurPos, mPrevPos;
	float					mRadius;	
};