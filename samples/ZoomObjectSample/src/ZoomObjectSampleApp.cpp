#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"


#include "TouchManager.h"
#include "TouchDrivers/Mouse.h"
#include "TouchDrivers/TUIO.h"
#include "TouchObjects/PanZoom/PanZoomObject.h"
#include "TouchObjects/PanZoom/PanZoomView.h"
using namespace ci;
using namespace ci::app;
using namespace std;
using namespace touchObject;

class ZoomObjectSampleApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

	Tuio mTouchConnection;
	Mouse mMouseConnection;
	PanZoomViewRef  panZoomViewRef;
	PanZoomViewRef  panZoomViewRef1;
};

void ZoomObjectSampleApp::setup()
{

	//Handle Connections for mouse and tuio
	mTouchConnection.connect();
	mMouseConnection.connect(true);


	ci::gl::TextureRef mImageTextureRef;
	fs::path                path(getAssetPath("motorcycle.jpg"));
	if (fs::exists(path)){ mImageTextureRef = cinder::gl::Texture::create(ci::loadImage(path)); }
	PanZoomObjectRef panZoomObjectRef = PanZoomObject::createZoomableImage(mImageTextureRef, 0.50, 2.0);
	panZoomViewRef = PanZoomView::create(Vec2f(100, 100), Vec2f(400, 400));
	panZoomViewRef->addObject(panZoomObjectRef);

	/*
	ci::gl::TextureRef mImageTextureRef1;
	fs::path                path1(getAssetPath("motorcycle1.jpg"));
	if (fs::exists(path1)){ mImageTextureRef1 = cinder::gl::Texture::create(ci::loadImage(path1)); }
	PanZoomObjectRef panZoomObjectRef1 = PanZoomObject::createZoomableImage(mImageTextureRef1, 0.5, 1.5);
	panZoomViewRef1 = PanZoomView::create(Vec2f(500, 25), Vec2f(400, 400));
	panZoomViewRef1->addObject(panZoomObjectRef1);
	*/
}

void ZoomObjectSampleApp::mouseDown( MouseEvent event )
{
}

void ZoomObjectSampleApp::update()
{
	TouchManager::getInstance()->update(); 
	panZoomViewRef->update();
	//panZoomViewRef1->update();
}

void ZoomObjectSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	//panZoomObjectRef->draw();

	panZoomViewRef->draw();
	//panZoomViewRef1->draw();
	TouchManager::getInstance()->draw();
	//Draw Framerate
	gl::drawString(to_string(getAverageFps()), Vec2f(0.0f, 0.0f), ColorA(1.0f, 0.0f, 0.0f, 1.0f), Font("Arial", 20.0f));
}

CINDER_APP_NATIVE( ZoomObjectSampleApp, RendererGl )
