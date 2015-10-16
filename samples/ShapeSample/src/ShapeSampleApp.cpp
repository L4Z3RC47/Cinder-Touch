#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchObjects/BaseButton.h"
#include "cinder/rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

class ShapeSampleApp : public App {
  public:
	void	setup() override;
	void	update() override;
	void	createButton1();
	void	createButton2();
	void	createButton3();
	void	draw() override;

private:
	void	button1Callback(touchObject::TouchObjectRef);
	void	button2Callback(touchObject::TouchObjectRef);
	void	button3Callback(touchObject::TouchObjectRef);

	Mouse		mConnection;
	ButtonRef	mButtonRef;

	std::vector<ButtonRef> mWeirdlyShapedButtons;
};

void ShapeSampleApp::setup(){
	mConnection.connect();
	gl::enableAlphaBlending();

	createButton1();
	createButton2();
	createButton3();
}

void ShapeSampleApp::update(){
	TouchManager::getInstance()->update();
}


void ShapeSampleApp::createButton1(){
	//option to send in values like normal, to create a quick rectangle button
	ButtonRef btn = BaseButton::create(vec2(0.0f, 30.0f), vec2(400.0f, 400.0f), std::bind(&ShapeSampleApp::button1Callback, this, std::placeholders::_1));
	btn->setObjectColor(ColorA(1.0f, 0.0f, 0.0f));
	mWeirdlyShapedButtons.push_back(btn);
}


void ShapeSampleApp::createButton2(){
	vector<vec2> coordinates;
	coordinates.push_back(vec2(0.0f, 0.0f));
	coordinates.push_back(vec2(300.0f, 0.0f));
	coordinates.push_back(vec2(200.0f, 200.0f));
	coordinates.push_back(vec2(100.0f, 500.0f));
	coordinates.push_back(vec2(400.0f, 600.0f));
	coordinates.push_back(vec2(200.0f, 600.0f));
	coordinates.push_back(vec2(0.0f, 600.0f));
	coordinates.push_back(vec2(0.0f, 300.0f));
	coordinates.push_back(vec2(100.0f, 200.0f));

	//option to send in coordinates to create any shape
	ButtonRef btn = BaseButton::create(coordinates, std::bind(&ShapeSampleApp::button2Callback, this, std::placeholders::_1));
	mWeirdlyShapedButtons.push_back(btn);
}

void ShapeSampleApp::createButton3(){
	//option to send in values like normal, to create a quick rectangle button
	ButtonRef btn = BaseButton::create(vec2(0, 0), 200.0f, std::bind(&ShapeSampleApp::button3Callback, this, std::placeholders::_1));
	btn->setObjectColor(ColorA(0.0f, 0.0f, 1.0f));
	mWeirdlyShapedButtons.push_back(btn);
}

void ShapeSampleApp::button1Callback(touchObject::TouchObjectRef){
	console() << "button1Callback" << endl;
	//Set the button to a random color
	float r = randFloat(0.0f, 1.0f);
	float g = randFloat(0.0f, 1.0f);
	float b = randFloat(0.0f, 1.0f);

	mWeirdlyShapedButtons[0]->setObjectColor(ColorA(r, g, b));
}

void ShapeSampleApp::button2Callback(touchObject::TouchObjectRef){
	console() << "button2Callback" << endl;
	//Set the button to a random color
	float r = randFloat(100.0f, 120.0f);
	float g = randFloat(50.0f, 100.0f);
	float b = randFloat(0.0f, 1.0f);

	mWeirdlyShapedButtons[1]->setObjectColor(ColorA(r, g, b));
}

void ShapeSampleApp::button3Callback(touchObject::TouchObjectRef){
	console() << "button3Callback" << endl;
	//Set the button to a random color
	float r = randFloat(0.0f, 20.0f);
	float g = randFloat(50.0f, 100.0f);
	float b = randFloat(0.0f, 1.0f);

	mWeirdlyShapedButtons[2]->setObjectColor(ColorA(r, g, b));
}

void ShapeSampleApp::draw(){
	gl::clear( Color( 0, 0, 0 ) ); 

	//if you don't translate the space, you can just draw your object
	mWeirdlyShapedButtons[0]->draw();
	
	//if you do translate space, you have to tell the object you're touching
	vec2 translatePos = vec2(100.0f, 100.0f);
	gl::pushMatrices(); {
		gl::translate(translatePos);
		mWeirdlyShapedButtons[1]->setTranslationPosition(translatePos);
		mWeirdlyShapedButtons[1]->draw();
	}gl::popMatrices();

	translatePos = vec2(100.0f, 100.0f);
	gl::pushMatrices(); {
		gl::translate(translatePos);
		mWeirdlyShapedButtons[2]->setTranslationPosition(translatePos);
		mWeirdlyShapedButtons[2]->draw();
	}gl::popMatrices();


	//draw the touches. The ID of the mouse will always be -1.
	TouchManager::getInstance()->draw();
}

CINDER_APP( ShapeSampleApp, RendererGl )
