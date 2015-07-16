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

	typedef std::shared_ptr<class		ScrollViewCellButton>		ScrollViewCellButtonRef;
	typedef std::shared_ptr<const class ScrollViewCellButton>		ScrollViewCellButtonConstRef;
	typedef std::weak_ptr  <class		ScrollViewCellButton>		ScrollViewCellButtonWeakRef;


	class ScrollViewCellButton : public touchObject::ScrollViewCell{
	public:

		ScrollViewCellButton();
		static ScrollViewCellButtonRef create(cinder::Vec2f size);
		void buttonCallback(touchObject::TouchObjectRef);
		virtual void				setPosition(const cinder::Vec2f &pt);
		virtual void	tapEventHandler(int touchID, const cinder::Vec2f &touchPnt, TouchType touchType);

		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());

		ButtonRef mButtonRef;


		bool mToggled;
			
	};
}
