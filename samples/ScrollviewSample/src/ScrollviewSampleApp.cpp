#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"


#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchObjects/ScrollView/ScrollView.h"
#include "TouchObjects/ScrollView/ScrollViewSection.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;
class ScrollviewSampleApp : public AppNative {
  public:
	void setup();

	void update();
	void draw();

	Mouse mMouseConnection;
	ScrollViewRef mScrollViewRef;
};

void ScrollviewSampleApp::setup()
{
	mMouseConnection.connect();
	mScrollViewRef = ScrollView::create(Vec2f(0, 0), Vec2f(200, 400), Vec2f(0, 0), ScrollView::ScrollViewType::Continuous, ScrollView::ScrollViewOrientation::Vertical);

	for (int i = 0; i < 3; i++){
		ScrollViewSectionRef section = ScrollViewSection::create(Vec2f(200, 100));
		Color color;

		if (i == 0)color = Color(255, 0, 0);
		if (i == 1)color = Color(255, 255, 0);
		if (i == 2)color = Color(0, 0, 255);
		
		section->setBackgroundColor(color);
		
		
		mScrollViewRef->addSection(section);

	}

}


void ScrollviewSampleApp::update()
{


		TouchManager::getInstance()->update();
		mScrollViewRef->update();

}

void ScrollviewSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	mScrollViewRef->draw();
}

CINDER_APP_NATIVE( ScrollviewSampleApp, RendererGl )
