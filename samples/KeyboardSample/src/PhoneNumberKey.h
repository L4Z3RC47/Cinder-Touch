#pragma once
#include "cinder/app/AppBasic.h"
#include "TouchObjects/Keyboard/Key.h"

namespace touchObject{

	//References
	typedef std::shared_ptr<class		PhoneNumberKey>		PhoneNumberKeyRef;
	typedef std::shared_ptr<const class PhoneNumberKey>		PhoneNumberKeyConstRef;
	typedef std::weak_ptr  <class		PhoneNumberKey>		PhoneNumberKeyWeakRef;


	class PhoneNumberKey :public Key  {
	public:
		PhoneNumberKey();
		virtual ~PhoneNumberKey(){};
		static  PhoneNumberKeyRef	create(const cinder::Vec2f &pos, const cinder::Vec2i &size, const std::string &keyString);
		virtual void setKeyString(const std::string &keyString);
		void draw(const cinder::Vec2f &parentTranslatePos = cinder::Vec2f(0,0));

	};
};