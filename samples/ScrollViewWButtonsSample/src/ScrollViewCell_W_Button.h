//
//  ScrollViewCellButton.h
//  cinder_gui_interface
//
//  Created by Paul Rudolph on 7/22/13.
//
//
#pragma once
#include "TouchObjects/ScrollView/ScrollViewCell.h"
#include "TouchObjects/Button/BaseButton.h"

namespace touchObject {

	typedef std::shared_ptr<class		ScrollViewCell_W_Button>		ScrollViewCell_W_ButtonRef;
	typedef std::shared_ptr<const class ScrollViewCell_W_Button>		ScrollViewCell_W_ButtonConstRef;
	typedef std::weak_ptr  <class		ScrollViewCell_W_Button>		ScrollViewCell_W_ButtonWeakRef;


	class ScrollViewCell_W_Button : public touchObject::ScrollViewCell{
	public:

		ScrollViewCell_W_Button();
		static ScrollViewCell_W_ButtonRef create(cinder::Vec2f size);
		void buttonCallback(touchObject::TouchObjectRef obj);
		virtual void				setPosition(const cinder::Vec2f &pt);
		//virtual void	tapEventHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);
		virtual void				handleTouchCanceled(touchObject::TouchObjectRef obj, int touchId);

		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());

		ButtonRef mButtonRef;


		bool mToggled;
			
	};
}
