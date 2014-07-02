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

	class BaseButton :public touchObject::BaseTouchObject{
	public://anyone can get access to this stuff with the "." accessor

	

		virtual ~BaseButton();

		static  ButtonRef			create(cinder::Vec2f pos, cinder::Vec2f size, std::function <void(touchObject::TouchObjectRef)>callBackFn = std::function <void(touchObject::TouchObjectRef)>());
		//Drawing Functions
		virtual void				draw();

		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType);
		virtual void				touchesMovedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType);

		void						setCallBackFn(std::function <void(touchObject::TouchObjectRef)> fn)     { mOnSelectSignal.connect(fn); };
		const boost::signals2::signal<void(touchObject::TouchObjectRef)>&			getSelectedSignal()				{ return mOnSelectSignal; }


	protected:
		bool														mTouchCanceled;
		boost::signals2::signal<void(touchObject::TouchObjectRef)>  mOnSelectSignal;

	private:
		BaseButton();
	};
}