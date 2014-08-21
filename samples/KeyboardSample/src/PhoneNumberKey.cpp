
#include "PhoneNumberKey.h"
//#include "Keyboard.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;


namespace touchObject{
	
	PhoneNumberKey::PhoneNumberKey() :Key()
	{}

	PhoneNumberKeyRef	PhoneNumberKey::create(const cinder::Vec2f &pos, const cinder::Vec2i &size, const std::string &keyString){
		PhoneNumberKeyRef PhonekeyObjRef(new PhoneNumberKey());
			PhonekeyObjRef->registerWithTouchMngr();
			PhonekeyObjRef->setPosition(pos);
			PhonekeyObjRef->setSize(size);
			PhonekeyObjRef->setKeyString(keyString);
			PhonekeyObjRef->setCallBackFn(std::bind(&Key::keyCallback, PhonekeyObjRef, std::placeholders::_1));
		return PhonekeyObjRef;
	}

	void PhoneNumberKey::setKeyString(const std::string &keyString){
		mKeyTextObjectRef = bci::TextObject::create(Vec2f(0, 0), Vec2f(0, 0), keyString);
		mKeyTextObjectRef->renderTextBox("phoneKey");
		Vec2f textureSize = mKeyTextObjectRef->getTexture()->getSize();
		mKeyTextObjectRef->setPosition(Vec2f(getWidth() / 2.0f - textureSize.x / 2.0f, getHeight() / 2.0f - textureSize.y / 2.0f));
	}

	void PhoneNumberKey::draw(const cinder::Vec2f &parentTranslatePos){

		if (parentTranslatePos != Vec2f::zero())
			setParentTranslatePosition(parentTranslatePos);

		gl::pushMatrices(); {
			gl::translate(mPosition );

			

			if (!mObjectTouchIDs.empty()){
				gl::color(44.0f/ 255.0f, 251.0f/ 255.0f, 232.0f/ 255.0f);
			
			}
			else{
				gl::color(1, 1, 1);
			}
			gl::lineWidth(2.25f);
			gl::drawStrokedRoundedRect(Rectf(0, 0, getWidth(), getHeight()), 5.0f);

	
			gl::color(1, 1, 1);
			gl::draw(mKeyTextObjectRef->getTexture(), mKeyTextObjectRef->getPosition());

		}gl::popMatrices();
	}
};