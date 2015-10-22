/**+---------------------------------------------------------------------------
Bluecadet Interactive 2014
Developers: Paul Rudolph & Stacey Martens
Contents:
Comments:
----------------------------------------------------------------------------*/

#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "BaseTouchObject.h"

namespace touchObject {

	typedef std::shared_ptr<class		BaseButton>		ButtonRef;
	typedef std::shared_ptr<const class BaseButton>		ButtonConstRef;
	typedef std::weak_ptr  <class		BaseButton>		ButtonWeakRef;

	class BaseButton :public touchObject::BaseTouchObject{
	public:
		//! Destructor
		virtual ~BaseButton();

		//! Create a rectangle
		static  ButtonRef			create(const cinder::vec2 &pos, cinder::vec2 size, std::function <void(touchObject::TouchObjectRef)>callBackFn = std::function <void(touchObject::TouchObjectRef)>());
		//! Create a circle
		static  ButtonRef			create(const cinder::vec2 &pos, float radius = 10.0f, std::function <void(touchObject::TouchObjectRef)>callBackFn = std::function <void(touchObject::TouchObjectRef)>());
		//! Create a wild shape by sending in all the coordinates
		static  ButtonRef			create(const std::vector<cinder::vec2> &coordinates, std::function <void(touchObject::TouchObjectRef)>callBackFn = std::function <void(touchObject::TouchObjectRef)>());

		virtual void				draw();

		virtual	void				touchesBeganHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType);
		virtual void				touchesMovedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType);
		virtual void				touchesEndedHandler(int touchID, const ci::vec2 &touchPnt, touchObject::TouchType);

		//! Set callback function for when this button is selected
		void						setCallBackFn(std::function <void(touchObject::TouchObjectRef)> fn)     { mOnSelectSignal.connect(fn); };
		const ci::signals::Signal<void(touchObject::TouchObjectRef)>&	getSelectedSignal()					{ return mOnSelectSignal; }

	protected:
		bool														mTouchCanceled;
		ci::signals::Signal< void(touchObject::TouchObjectRef) >	mOnSelectSignal;
	private:
		BaseButton();
	};
}