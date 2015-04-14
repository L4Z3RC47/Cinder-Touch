
#include "Key.h"
//#include "Keyboard.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;


namespace touchObject{
	
	Key::Key() :BaseButton(),
		mKeyType(Character)
	{
		
	}



	KeyRef	Key::create(const cinder::Vec2f &pos ,const cinder::Vec2i &size, const std::string &keyString){
		KeyRef keyObjRef(new Key());

		keyObjRef->registerWithTouchMngr();
		keyObjRef->setPosition(pos);
		keyObjRef->setSize(size);
		keyObjRef->setKeyString(keyString);
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



	void Key::setKeyString(const std::string &keyString){
		/*
		mKeyTextObjectRef = bci::TextObject::create(Vec2f(0,0), Vec2f(0,0), keyString);
		mKeyTextObjectRef->renderTextBox("character");
		Vec2f textureSize = mKeyTextObjectRef->getTexture()->getSize();
		mKeyTextObjectRef->setPosition(  Vec2f(getWidth() / 2.0f - textureSize.x / 2.0f, getHeight() / 2.0f - textureSize.y / 2.0f));
		*/
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

	void Key::draw(const cinder::Vec2f &parentTranslatePos){

		if (parentTranslatePos != Vec2f::zero())
			setParentTranslatePosition(parentTranslatePos);

		gl::pushMatrices(); {
			gl::translate(mPosition );

			gl::color(getObjectColor());
			drawDebugBox(true);//if translating, let the debug box know

			if (!mObjectTouchIDs.empty()){
				gl::drawSolidRect(Rectf(0, 0, getWidth(), getHeight()));
			}
	
			gl::color(1, 1, 1);
			//gl::draw(mKeyTextObjectRef->getTexture(), mKeyTextObjectRef->getPosition());

		}gl::popMatrices();
	}
};