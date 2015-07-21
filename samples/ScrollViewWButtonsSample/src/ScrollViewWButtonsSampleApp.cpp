#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchDrivers/TUIO.h"
#include "TouchObjects/ScrollView/ScrollView.h"
#include "ScrollViewCell_W_Button.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

class ScrollViewWButtonsSampleApp : public AppNative {
  public:
	void setup();
	void prepareSettings(Settings *settings);
	void update();
	void draw();

	Tuio mTouchConnection;
	Mouse mMouseConnection;
	ScrollViewRef mVerticalScrollViewRef; 



};

void ScrollViewWButtonsSampleApp::prepareSettings(Settings *settings){
	settings->setWindowSize(Vec2f(1000, 1000));
	settings->enableHighDensityDisplay();
}


void ScrollViewWButtonsSampleApp::setup(){

	//Handle Connections for mouse and tuio
	mTouchConnection.connect();
	mMouseConnection.connect();

	//Create a vertical Scrollview
	mVerticalScrollViewRef = ScrollView::create(Vec2f(50, 100), Vec2f(800, 800), ScrollView::ScrollViewType::Continuous, ScrollView::ScrollViewOrientation::Vertical);


	mVerticalScrollViewRef->setShouldClipSubviews(true);
	

	for (int i = 0; i < 6; i++){
		ScrollViewCell_W_ButtonRef cell = ScrollViewCell_W_Button::create(Vec2f(200, 200));

		Color color;

		switch (i){
			case 0:color = Color(1, 0, 0); break;
			case 1:color = Color(0, 1, 0); break;
			case 2:color = Color(0, 0, 1); break;
			case 3:color = Color(1, 1, 0); break;
			case 4:color = Color(1, 0, 1); break;
			case 5:color = Color(0, 1, 1); break;
		}



		cell->setObjectColor(color);
		mVerticalScrollViewRef->addCell(cell);
		
	}

}




void ScrollViewWButtonsSampleApp::update()
{

	TouchManager::getInstance()->update();
	//Scroll views need to be updated to handle easing
	mVerticalScrollViewRef->update();

}

void ScrollViewWButtonsSampleApp::draw()
{


	// clear out the window with black
	gl::clear(Color(0, 0, 0));
	//Draw the touches
	TouchManager::getInstance()->draw();

	mVerticalScrollViewRef->draw();

}

CINDER_APP_NATIVE( ScrollViewWButtonsSampleApp, RendererGl )

