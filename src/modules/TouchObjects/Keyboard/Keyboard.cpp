
#include "Keyboard.h"
//#include "Key.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;


namespace touchObject{

	Keyboard::Keyboard(): touchObject::BaseTouchObject(),
		mInputString("")
	{
		mKeyTextObjectRef = bci::TextObject::create(Vec2f(0, 0), Vec2i(300, 200), "ENTER NUMBER");
		mKeyTextObjectRef->renderTextBox();
	}

	KeyboardRef	Keyboard::create(const cinder::Vec2f &pos, const cinder::Vec2i &size){
		KeyboardRef keyboardObjRef(new Keyboard());


		keyboardObjRef->setPosition(pos);
		keyboardObjRef->setSize(size);


		return keyboardObjRef;
	}

	void Keyboard::addKey(std::shared_ptr<class	Key> keyRef){
		std::shared_ptr<Keyboard> keyboardRef = dynamic_pointer_cast<Keyboard>(shared_from_this());
		shared_from_this();
		keyRef->setKeyboard(keyboardRef);
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
		case KeyType::Character: {mInputString += keyRef->getKeyString();
									 break;
		}
		case KeyType::Submit: {console() << "Submit Pressed" << endl;
									 break;
		}
		case KeyType::Backspace: {
			 mInputString = mInputString.substr(0, mInputString.length() - 1);
									 break;
		}
		case KeyType::Clear: {
								 mInputString = "";
									 break;
		}
			
		
		}
		mKeyTextObjectRef->setString(mInputString);
		mKeyTextObjectRef->renderTextBox("textInput");
	}

	void Keyboard::draw(const cinder::Vec2f &parentTranslatePos){
		gl::pushMatrices(); {
			gl::translate(mPosition);

			gl::color(getObjectColor());
			drawDebugBox(true);//if translating, let the debug box know

			for (auto k : mKeys){
				k->draw(mPosition + parentTranslatePos);
			}

			
			gl::color(1, 1, 1);
			
				gl::draw(mKeyTextObjectRef->getTexture(), mKeyTextObjectRef->getPosition());
		
			

			//console() << "INPUT STRING:: " << mInputString << endl;

		}gl::popMatrices();
	}
};