#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"


#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchDrivers/TUIO.h"
#include "TouchObjects/ScrollView/ScrollView.h"
#include "TouchObjects/ScrollView/ScrollViewCell.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;
class ScrollviewSampleApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
	Tuio mTouchConnection;
	Mouse mMouseConnection;
	ScrollViewRef mVerticalScrollViewRef;
	ScrollViewRef mHorizontalScrollViewRef;
};

void ScrollviewSampleApp::setup()
{
	//Handle Connections for mouse and tuio
	mTouchConnection.connect();
	mMouseConnection.connect();

	//Create a vertical Scrollview
	mVerticalScrollViewRef = ScrollView::create(Vec2f(50, 100), Vec2f(400, 400), ScrollView::ScrollViewType::Continuous, ScrollView::ScrollViewOrientation::Vertical);
	mVerticalScrollViewRef->setShouldClipSubviews(false);

	for (int i = 0; i < 6; i++){
		ScrollViewCellRef cell = ScrollViewCell::create(Vec2f(100, 100));
		Color color;


		if (i == 0){
			color = Color(255, 0, 100);
			//cell->setTopPadding(25.0f);
		}
		else if (i == 1)color = Color(255, 255, 0);
		else if (i == 2){
			color = Color(0, 0, 255);
		}
		else{
			color = Color(1, 0, 0);
		}
		cell->setObjectColor(color);
		mVerticalScrollViewRef->addCell(cell);
	}

	//Create A Horizontal Scrollview
	
	mHorizontalScrollViewRef = ScrollView::create(Vec2f(500, 50), Vec2f(400, 400), ScrollView::ScrollViewType::NonContinuous, ScrollView::ScrollViewOrientation::Horizontal);
	mHorizontalScrollViewRef->setShouldClipSubviews(true);

	for (int i = 0; i < 5; i++){
		ScrollViewCellRef cell = ScrollViewCell::create(Vec2f(100, 100));
		Color color;

		if (i == 0){
			color = Color(255, 0, 100);
			//cell->setLeftPadding(25.0f);
		}
		else if (i == 1)color = Color(255, 255, 0);
		else if (i == 2){
			color = Color(0, 0, 255);
		}
		else{
			color = Color(1, 0, 0);
		}
		cell->setObjectColor(color);
		mHorizontalScrollViewRef->addCell(cell);
	}

	
}


void ScrollviewSampleApp::update()
{
		TouchManager::getInstance()->update();
		//Scroll views need to be updated to handle easing
		mVerticalScrollViewRef->update();
		mHorizontalScrollViewRef->update();
}

void ScrollviewSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	//Draw the touches
	TouchManager::getInstance()->draw();

	mVerticalScrollViewRef->draw();
	mHorizontalScrollViewRef->draw();
}

CINDER_APP_NATIVE( ScrollviewSampleApp, RendererGl )
