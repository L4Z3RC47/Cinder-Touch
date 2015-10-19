//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
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

class SampleObject : public touchObject::BaseTouchObject{
public:
	SampleObject();
	
	virtual void	draw(cinder::vec2 parentTranslatePos = cinder::vec2(0));

	//use virtual to override the BaseTouchObject calls
	//with these functions, we will drag the objects around on the screen
	virtual	void	touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	virtual void	touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType touchType);
	
	std::vector<std::shared_ptr<SampleObject> > mChildVector;

private:
	//keep track of object position
	ci::vec2	mCurPos, mPrevPos, mTranslationPosition;
};