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
	ScrollViewRef mScrollViewRef;
};

void ScrollviewSampleApp::setup()
{
	//Handle Connections for mouse and tuio
	mTouchConnection.connect();
	mMouseConnection.connect();
	mScrollViewRef = ScrollView::create(  Vec2f(50, 50), Vec2f(400, 400), ScrollView::ScrollViewType::Continuous, ScrollView::ScrollViewOrientation::Vertical);
	mScrollViewRef->setShouldClipSubviews(true);


	for (int i = 0; i < 10; i++){
		ScrollViewCellRef section = ScrollViewCell::create(Vec2f(200, 100));
		Color color;

		if (i == 0)color = Color(255, 0, 0);
		else if (i == 1)color = Color(255, 255, 0);
		else if (i == 2)color = Color(0, 0, 255);
		else{
			color = Color(1, 0, 0);
		}
		section->setObjectColor(color);
		mScrollViewRef->addSection(section);
	}

}


void ScrollviewSampleApp::update()
{
		TouchManager::getInstance()->update();
		//Scroll views need to be updated to handle easing
		mScrollViewRef->update();
}

void ScrollviewSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	//Draw the touches
	TouchManager::getInstance()->draw();

	mScrollViewRef->draw();
}

CINDER_APP_NATIVE( ScrollviewSampleApp, RendererGl )
