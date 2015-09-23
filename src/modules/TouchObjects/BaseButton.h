//+---------------------------------------------------------------------------
//  Bluecadet Interactive 2014
//	Developers: Paul Rudolph & Stacey Martens
//  Contents: 
//  Comments: 
//----------------------------------------------------------------------------

#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "BaseTouchObject.h"

namespace touchObject {

	typedef std::shared_ptr<class		BaseButton>		ButtonRef;
	typedef std::shared_ptr<const class BaseButton>		ButtonConstRef;
	typedef std::weak_ptr  <class		BaseButton>		ButtonWeakRef;

	class BaseButton :public touchObject::BaseTouchObject{
	public://anyone can get access to this stuff with the "." accessor

		virtual ~BaseButton();

		static  ButtonRef			create(ci::vec2 pos, ci::vec2 size, std::function <void(touchObject::TouchObjectRef)>callBackFn = std::function <void(touchObject::TouchObjectRef)>());
		
		//Drawing Functions
		virtual void				draw();

		virtual	void				touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType);
		virtual void				touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType);
		virtual void				touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType);

		void						setCallBackFn(std::function <void(touchObject::TouchObjectRef)> fn)     { mOnSelectSignal.connect(fn); };
		const ci::signals::Signal<void(touchObject::TouchObjectRef)>&	getSelectedSignal()					{ return mOnSelectSignal; }


	protected:
		bool												mTouchCanceled;
		ci::signals::Signal< void(touchObject::TouchObjectRef) >  mOnSelectSignal;

	private:
		BaseButton();
	};
}