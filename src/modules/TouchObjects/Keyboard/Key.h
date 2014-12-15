#pragma once
#include "cinder/app/AppBasic.h"
#include "TouchObjects/Button/BaseButton.h"
//#include "TextObject.h"
#include "../../../Cinder-samples/FIELD_KIOSK/FLDmapSample/src/TextUtils/TextObject.h"

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

		virtual	void				touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);
		virtual void				touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType);


		void draw(const cinder::Vec2f &parentTranslatePos  =cinder::Vec2f(0.0f,0.0f));
		virtual void setKeyString(const std::string &keyString);
		void keyCallback(touchObject::TouchObjectRef obj );
		void setKeyboard(std::shared_ptr<class	Keyboard> keyboardRef);
		std::string getKeyString(){ return mKeyTextObjectRef->getTextString(); };

		void setKeyType(KeyType type){ mKeyType = type; };
		KeyType getKeyType(){ return  mKeyType; };
	protected:

		bci::TextObjectRef				mKeyTextObjectRef;
		std::shared_ptr<class Keyboard> mKeyboardRef;
		KeyType mKeyType;
	};
};