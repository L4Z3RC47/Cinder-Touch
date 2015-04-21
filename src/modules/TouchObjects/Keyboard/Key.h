#pragma once
#include "cinder/app/AppBasic.h"
#include "TouchObjects/Button/BaseButton.h"
#include "Keyboard.h"

namespace touchObject{
	//References
	typedef std::shared_ptr<class		Key>		KeyRef;
	typedef std::shared_ptr<const class Key>		KeyConstRef;
	typedef std::weak_ptr  <class		Key>		KeyWeakRef;

	enum KeyType{ Character, Submit, Backspace, Clear };

	class Keyboard;
	class Key :public touchObject::BaseButton  {
	public:
		Key();
		virtual ~Key(){};
		static  KeyRef	create(const cinder::Vec2f &pos , const cinder::Vec2i &size, const std::string &keyString);

		//Touch handling
		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		
		void keyCallback(touchObject::TouchObjectRef obj);
		void setKeyboard(std::shared_ptr<class	Keyboard> keyboardRef);


		//Drawing
		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());
		
		//Properties
		
		//Type
		void setKeyType(KeyType type){ mKeyType = type; };
		KeyType getKeyType(){ return  mKeyType; };
		
		//Label
		virtual void setLabelText(const std::string &labelText){ mKeyLabelText = labelText; };
		std::string	 getLabelText(){ return mKeyLabelText; };

		//Font
		virtual void setLabelFont(const ci::Font &labelFont){ mLabelFont = labelFont; };
		ci::Font	 getLabelFont(){ return mLabelFont; };

		//Color
		virtual void setLabelColor(const ci::ColorA &labelColor){ mLabelColor = labelColor; };
		ci::ColorA	 getLabelColor(){ return mLabelColor; };

	protected:
		std::shared_ptr<class Keyboard> mKeyboardRef;
		KeyType mKeyType;
		
		//Label
		std::string mKeyLabelText;
		ci::Font mLabelFont;
		ci::ColorA mLabelColor;

	};
};