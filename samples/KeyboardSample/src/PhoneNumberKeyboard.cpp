
#include "PhoneNumberKeyboard.h"
#include "PhoneNumberKey.h"


using namespace std;
using namespace ci;
using namespace ci::app;
using namespace boost;


namespace touchObject{
	std::vector<std::string> PhoneNumberKeyboard::InvalidAreaCodes = []{ 
		vector<std::string> v;
	v.push_back("800");
	v.push_back("888");
	v.push_back("555");
	v.push_back("900");
	v.push_back("456");
	return v; }();

	PhoneNumberKeyboard::PhoneNumberKeyboard() : Keyboard()
	{
		
	}
	
	PhoneNumberKeyboardRef	PhoneNumberKeyboard::create(const cinder::Vec2f &pos, const cinder::Vec2i &size){
		PhoneNumberKeyboardRef phoneKeyboardObjRef(new PhoneNumberKeyboard());
		phoneKeyboardObjRef->setPosition(pos);
		phoneKeyboardObjRef->setSize(size);
		layoutKeys(phoneKeyboardObjRef);
		return phoneKeyboardObjRef;
	}

	void PhoneNumberKeyboard::layoutKeys(PhoneNumberKeyboardRef keyboardRef){
		
		float buttonWidth = 50.0f;
		float buttonHeight = 50.0f;
		float verticalPadding = 100.0f;
		float horizontalPadding = 25.0f;

		float verticalSpacing =75.0f;
		float horizontalSpacing = 75.0f;
		float yPos = verticalPadding;
		float xPos = horizontalPadding;
		float buttonsPerRow = 3;
	

		int buttonsOnRow = 0;
		for (int i = 1; i <= 10; i++){
			if (buttonsOnRow == buttonsPerRow){
				buttonsOnRow = 0;
				yPos += verticalSpacing;
				xPos = horizontalPadding;
			}

			if (i < 10){
				PhoneNumberKeyRef keyRef = PhoneNumberKey::create(Vec2f(xPos, yPos), Vec2f(buttonWidth, buttonHeight), to_string(i));
				keyboardRef->addKey(keyRef);
				xPos += horizontalSpacing;
				buttonsOnRow++;
			}
			if (i == 10){
				PhoneNumberKeyRef keyRef = PhoneNumberKey::create(Vec2f(horizontalPadding  + horizontalSpacing, yPos), Vec2f(buttonWidth, buttonHeight), "0");
				keyboardRef->addKey(keyRef);

			}

		}
		


		PhoneNumberKeyRef clearKeyRef = PhoneNumberKey::create(Vec2f(horizontalPadding, yPos), Vec2f(50, 50), "Clear");
		clearKeyRef->setKeyType(touchObject::KeyType::Clear);
		keyboardRef->addKey(clearKeyRef);

		PhoneNumberKeyRef bkspcKeyRef = PhoneNumberKey::create(Vec2f((2 * buttonWidth) + horizontalSpacing, yPos), Vec2f(50, 50), "Backspace");
		bkspcKeyRef->setKeyType(touchObject::KeyType::Backspace);
		keyboardRef->addKey(bkspcKeyRef);


		PhoneNumberKeyRef submitKeyRef = PhoneNumberKey::create(Vec2f(horizontalPadding + buttonWidth, yPos + verticalSpacing), Vec2f(2 * buttonWidth, buttonHeight), "Submit");
		submitKeyRef->setKeyType(touchObject::KeyType::Submit);
		keyboardRef->addKey(submitKeyRef);
	
	}

	void	PhoneNumberKeyboard::keyPressed(std::shared_ptr<class Key> keyRef){
		
			switch (keyRef->getKeyType()){
			case KeyType::Character: {if (mInputString.length() < 10) mInputString += keyRef->getKeyString();
				break;
			}
			case KeyType::Submit: {
									  if (santizePhoneNumber()){
										
										  mSubmitFn(mInputString);
										  mInputString = "";
									  }
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

	bool PhoneNumberKeyboard::santizePhoneNumber(){

		//Rules have been created based on NANPA North american Numbering Plan Administration : http://www.nanpa.com/index.html
		bool areaCodeValid = false;
		bool exchangeCodeValid = false;
		bool suscriberNumValid = false;
		bool numberLengthValid = false;

		if (mInputString.length() == 10){
			numberLengthValid = true;
			//Terms are used from North American Numbering Plan
			std::string areaCode = mInputString.substr(0, 3);//NPA
			std::string exchangeCode = mInputString.substr(3, 3);//NXX
			std::string SuscriberNumber = mInputString.substr(6, 4);//SUSCRIBER

			//Check Area Code
			console() << "areaCode : " << areaCode << endl;
			int areacode[] = { areaCode.at(0) - '0', areaCode.at(2) - '0', areaCode.at(2) - '0' };
			if ((areacode[0] >= 2 && areacode[0] <= 9) && //First digit must be between 2 and 9  
				(areacode[1] != 1 && areacode[2] != 1) && //Check that area code is an ERC (service code)
				(areacode[9]!=9) &&						  // check that area is not an expansion number 
				(areacode[0] != 3 && areacode[1] != 7) && //Check for not a reserved purpose
				(areacode[0] != 9 && areacode[1] != 6) &&//Check for not a reserved purpose
				(find(InvalidAreaCodes.begin(), InvalidAreaCodes.end(), areaCode) == InvalidAreaCodes.end())){

				areaCodeValid = true;

			}
			else{
				console() << "Area Code is INVALID" << endl;
			}


			//Check Exchange Code
			console() << "exchangeCode : " << exchangeCode << endl;
			int exCode[] = { exchangeCode.at(0) - '0', exchangeCode.at(1) - '0', exchangeCode.at(2) - '0' };
			if ((exCode[0] >= 2 && exCode[0] <= 9)&&
				(exCode[1] != 1 && exCode[2] != 1)  //Check that area code is an ERC (service code)
				){
				exchangeCodeValid = true;
			
			}
			else{
				console() << "Exchange Code Invalid" << endl;
			}


			//Check suscriber Number

			console() << "Suscriber : " << SuscriberNumber << endl;
			if (true){
				suscriberNumValid = true;
			}
			else{
				console() << "Suscriber Number Invalid" << endl;
			}

		}
		else{
			console() << "Phone number is to short" << endl;
			
		}

		return 		areaCodeValid && exchangeCodeValid && suscriberNumValid && numberLengthValid;
	}

	void PhoneNumberKeyboard::draw(const cinder::Vec2f &parentTranslatePos){
		gl::pushMatrices(); {
			gl::translate(mPosition);

			gl::color(getObjectColor());
			drawDebugBox(true);//if translating, let the debug box know
			
			for (auto k : mKeys){
				PhoneNumberKeyRef phoneKeyRef = std::dynamic_pointer_cast<PhoneNumberKey>(k);
				if (phoneKeyRef){
					phoneKeyRef->draw(mPosition + parentTranslatePos);
				}
			}

			
			gl::color(1, 1, 1);
			gl::draw(mKeyTextObjectRef->getTexture(), mKeyTextObjectRef->getPosition());
		
			

			//console() << "INPUT STRING:: " << mInputString << endl;
			
		}gl::popMatrices();
	}
};