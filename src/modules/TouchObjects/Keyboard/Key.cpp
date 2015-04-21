
#include "Key.h"
//#include "Keyboard.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;


namespace touchObject{
	
	Key::Key() :BaseButton(),
		mKeyType(Character),
		mKeyLabelText(""),
		mLabelFont(Font("arial",12.0f)),
		mLabelColor(ColorA(1,1,1,1))
	{
		
	}



	KeyRef	Key::create(const cinder::Vec2f &pos ,const cinder::Vec2i &size, const std::string &keyString){
		KeyRef keyObjRef(new Key());

		keyObjRef->registerWithTouchMngr();
		keyObjRef->setPosition(pos);
		keyObjRef->setSize(size);
		keyObjRef->setLabelText(keyString);
		keyObjRef->setCallBackFn(std::bind(&Key::keyCallback, keyObjRef, std::placeholders::_1));
		return keyObjRef;
	}

	void	Key::touchesBeganHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
			if (mKeyboardRef){
				std::shared_ptr<Key> keyRef = dynamic_pointer_cast<Key>(shared_from_this());
				mKeyboardRef->keyHasTouchDown(true, keyRef);
			}
			BaseButton::touchesBeganHandler(touchID, touchPnt, touchType);
	}
	
	void	Key::touchesEndedHandler(int touchID, const cinder::Vec2f &touchPnt, touchObject::TouchType touchType){
			if (mKeyboardRef){
				std::shared_ptr<Key> keyRef = dynamic_pointer_cast<Key>(shared_from_this());
				mKeyboardRef->keyHasTouchDown(false, keyRef);
			}
			BaseButton::touchesEndedHandler(touchID, touchPnt, touchType);
	}


	void Key::keyCallback(touchObject::TouchObjectRef obj){
	
		if (mKeyboardRef){
			std::shared_ptr<Key> keyRef = dynamic_pointer_cast<Key>(shared_from_this());
			mKeyboardRef->keyPressed(keyRef);
		}
	}
	
	void Key::setKeyboard(std::shared_ptr<class	Keyboard> keyboardRef){
		mKeyboardRef = keyboardRef; 
	}
	
	void	Key::draw(cinder::Vec2f translationOffset){

		gl::pushMatrices(); {
			gl::translate(translationOffset);



			if (!mObjectTouchIDs.empty()){
				gl::color(44.0f / 255.0f, 251.0f / 255.0f, 232.0f / 255.0f);

			}
			else{
				gl::color(1, 1, 1);
			}
			gl::lineWidth(2.25f);
			gl::drawStrokedRoundedRect(getRect(LOCAL), 5.0f);


			gl::color(1, 1, 1);
			gl::drawStringCentered(mKeyLabelText, getCenter(), mLabelColor, mLabelFont);


		}gl::popMatrices();
	}
};



