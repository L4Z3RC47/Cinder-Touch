#pragma once
#include "cinder/app/AppBasic.h"
#include  "BaseTouchObject.h"
#include "TouchObjects/Keyboard/Keyboard.h"

namespace touchObject{

	//References
	typedef std::shared_ptr<class		PhoneNumberKeyboard>		PhoneNumberKeyboardRef;
	typedef std::shared_ptr<const class PhoneNumberKeyboard>		PhoneNumberKeyboardConstRef;
	typedef std::weak_ptr  <class		PhoneNumberKeyboard>		PhoneNumberKeyboardWeakRef;
	typedef std::function  <void(std::string)>						SubmitFunction;


	class PhoneNumberKeyboard : public Keyboard {
	
	public:
		PhoneNumberKeyboard();
		static  PhoneNumberKeyboardRef	create(const cinder::Vec2f &pos, const cinder::Vec2i &size);

		virtual void	keyPressed(std::shared_ptr<class Key> touchedKey);
		void draw(const cinder::Vec2f &parentTranslatePos = cinder::Vec2f(0, 0));
		void setSubmitFunction(SubmitFunction fn)     { mSubmitFn = fn; };

	private:
		static void layoutKeys(PhoneNumberKeyboardRef keyboardRef);
		bool santizePhoneNumber();
		SubmitFunction mSubmitFn;
		static std::vector<std::string> InvalidAreaCodes;
	};
};