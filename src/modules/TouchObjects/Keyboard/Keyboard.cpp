
#include "Keyboard.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;


namespace touchObject{

	Keyboard::Keyboard(): touchObject::BaseTouchObject(),
		mInputText(""),
		mInputTextFont(Font("arial", 16.0f)),
		mInputTextColor(ColorA(1, 1, 1, 1))
	{

	}

	KeyboardRef	Keyboard::create(const cinder::Vec2f &pos, const cinder::Vec2i &size){
		KeyboardRef keyboardObjRef(new Keyboard());


		keyboardObjRef->setPosition(pos);
		
		keyboardObjRef->setSize(size);
		keyboardObjRef->setObjectColor(ci::ColorA(1, 0, 0, 1));

		return keyboardObjRef;
	}

	void Keyboard::addKey(std::shared_ptr<class	Key> keyRef){
		std::shared_ptr<Keyboard> keyboardRef = dynamic_pointer_cast<Keyboard>(shared_from_this());
		shared_from_this();
		keyRef->setKeyboard(keyboardRef);
		keyRef->setParentPosition(getGlobalPosition());
		mKeys.push_back(keyRef);
	}

	/*
		If a key has a down then lock out other keys
	*/
	void	Keyboard::keyHasTouchDown(bool state, std::shared_ptr<class Key> touchedKey){
		for (auto key : mKeys)key->setAcceptTouch(!state);
	}
	void	Keyboard::keyPressed(std::shared_ptr<class Key> keyRef){
		
		switch (keyRef->getKeyType()){
		case KeyType::Character: {
			mInputText += keyRef->getLabelText();
			break;
		}
		case KeyType::Submit: {
			mSubmitFn(mInputText);
			break;
		}
		case KeyType::Backspace: {
			mInputText = mInputText.substr(0, mInputText.length() - 1);
			break;
		}
		case KeyType::Clear: {
			mInputText = "";
			break;
		}
			
		
		}
	}

	void Keyboard::draw(cinder::Vec2f translationOffset){
		gl::pushMatrices(); {
			gl::translate(translationOffset);

			gl::color(getObjectColor());
			drawDebugBox();//if translating, let the debug box know

			for (auto key : mKeys){
				key->draw(getPosition());
			}

			
			gl::color(1, 1, 1);
			
			gl::drawStringCentered(mInputText, Vec2f(getWidth() / 2.0f, 50.0f), mInputTextColor, mInputTextFont);

	
			//console() << "INPUT STRING:: " << mInputString << endl;

		}gl::popMatrices();
	}
};