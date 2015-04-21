#pragma once
#include "cinder/app/AppBasic.h"
#include  "BaseTouchObject.h"
#include "Key.h"

namespace touchObject{
	//References
	typedef std::shared_ptr<class		Keyboard>		KeyboardRef;
	typedef std::shared_ptr<const class Keyboard>		KeyboardConstRef;
	typedef std::weak_ptr  <class		Keyboard>		KeyboardWeakRef;
	typedef std::function  <void(std::string)>			SubmitFunction;

	//class Key;
	class Keyboard : public touchObject::BaseTouchObject {
	
	public:
		Keyboard();
		virtual ~Keyboard(){};
		static  KeyboardRef	create(const cinder::Vec2f &pos, const cinder::Vec2i &size);
		void	addKey(std::shared_ptr<class Key> keyRef);
		virtual void	keyPressed(std::shared_ptr<class Key> touchedKey);
		virtual void	draw(cinder::Vec2f translationOffset = cinder::Vec2f::zero());
		void	keyHasTouchDown(bool state, std::shared_ptr<class Key> key);
		void	setSubmitFunction(SubmitFunction fn)     { mSubmitFn = fn; };

		//Input Text Label Properties
		std::string	 getInputFieldText(){ return mInputText; };

		//Font
		virtual void setInputFieldFont(const ci::Font &fnt){ mInputTextFont = fnt; };
		ci::Font	 getInputFieldFont(){ return mInputTextFont; };

		//Color
		virtual void setInputFieldColor(const ci::ColorA &clr){ mInputTextColor = clr; };
		ci::ColorA	 getInputFieldColor(){ return mInputTextColor; };
	
protected:
		SubmitFunction mSubmitFn;
		std::vector<std::shared_ptr<class Key>> mKeys;
	
		std::string mInputText;
		ci::Font mInputTextFont;
		ci::ColorA mInputTextColor;
	};
};