//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/AppNative.h"
#include "BaseTouchObject.h"
namespace touchObject {

	typedef std::shared_ptr<class		BaseButton>		ButtonRef;
	typedef std::shared_ptr<const class BaseButton>		ButtonConstRef;
	typedef std::weak_ptr  <class		BaseButton>		ButtonWeakRef;

	typedef std::function <void(touchObject::TouchObjectRef)>		CallbackFunction;

	class BaseButton :public touchObject::BaseTouchObject{
	public://anyone can get access to this stuff with the "." accessor

	
		BaseButton(); //This needs to be public so subclasses of the button can be made
		virtual ~BaseButton();

		static  ButtonRef			create(cinder::Vec2f pos, cinder::Vec2f size, CallbackFunction callBackFn = CallbackFunction());
		//Drawing Functions
		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());


		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);

		void						setCallBackFn(CallbackFunction fn)     { mCallbackFunction=fn; };
	

	protected:
		bool														mTouchCanceled;
		CallbackFunction											mCallbackFunction;
	};
}