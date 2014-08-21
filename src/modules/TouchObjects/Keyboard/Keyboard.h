#pragma once
#include "cinder/app/AppBasic.h"
#include  "BaseTouchObject.h"
#include "Key.h"

namespace touchObject{
	//References
	typedef std::shared_ptr<class		Keyboard>		KeyboardRef;
	typedef std::shared_ptr<const class Keyboard>		KeyboardConstRef;
	typedef std::weak_ptr  <class		Keyboard>		KeyboardWeakRef;

	//class Key;
	class Keyboard : public touchObject::BaseTouchObject {
	
	public:
		Keyboard();
		//virtual ~Keyboard();
		static  KeyboardRef	create(const cinder::Vec2f &pos, const cinder::Vec2i &size);
		void	addKey(std::shared_ptr<class Key> keyRef);
		virtual void	keyPressed(std::shared_ptr<class Key> touchedKey);
		void	draw(const cinder::Vec2f &parentTranslatePos  =cinder::Vec2f(0.0f,0.0f));
		void	keyHasTouchDown(bool state, std::shared_ptr<class Key> key);
	
protected:
		
		std::vector<std::shared_ptr<class		Key>> mKeys;
		std::string mInputString;

		bci::TextObjectRef				mKeyTextObjectRef;

	};
};