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

	void keyDown(KeyEvent event);
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

	//Load an image
	ci::gl::TextureRef mImageTextureRef;
	fs::path                path(getAssetPath("motorcycle.jpg"));
	if (fs::exists(path)){ mImageTextureRef = cinder::gl::Texture::create(ci::loadImage(path)); }

	//create Panzoom object and view
	PanZoomObjectRef panZoomObjectRef = PanZoomObject::createZoomableImage(mImageTextureRef, 0.50, 2.0);
	panZoomViewRef = PanZoomView::create(Vec2f(100, 100), Vec2f(700, 700));
	panZoomViewRef->addObject(panZoomObjectRef);

	
}

void ZoomObjectSampleApp::keyDown(KeyEvent event){
	if (event.getChar() == '1'){
		panZoomViewRef->setContentOffset(Vec2f(50, 50),true);
	}
}

void ZoomObjectSampleApp::update()
{
	//We have to update the manager to have touches work
	TouchManager::getInstance()->update(); 
	
	//Update the panzoom object
	panZoomViewRef->update();

}

void ZoomObjectSampleApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 


	panZoomViewRef->draw();

	TouchManager::getInstance()->draw();
	//Draw Framerate
	gl::drawString(to_string(getAverageFps()), Vec2f(0.0f, 0.0f), ColorA(1.0f, 0.0f, 0.0f, 1.0f), Font("Arial", 20.0f));
}

CINDER_APP_NATIVE( ZoomObjectSampleApp, RendererGl )
