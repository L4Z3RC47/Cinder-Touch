//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------
#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

//touches
#include "BaseTouchObject.h"

class SampleObject : public touchObject::BaseTouchObject{
public:
	SampleObject();
	
	void			setup(const cinder::Vec2f &position, const cinder::Vec2f &size, const cinder::ColorA &color);
	virtual void	draw(cinder::Vec2f parentTranslatePos = cinder::Vec2f::zero());

	//use virtual to override the BaseTouchObject calls
	//with these functions, we will drag the objects around on the screen
	virtual	void	touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
	
	std::vector<std::shared_ptr<SampleObject> > mChildVector;

private:
	//keep track of object position
	cinder::Vec2f	mCurPos, mPrevPos;
	cinder::Vec2f	mParentTranslatePos;

};