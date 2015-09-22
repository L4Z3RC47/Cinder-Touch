#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Button_vc2013App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void Button_vc2013App::setup()
{
}

void Button_vc2013App::mouseDown( MouseEvent event )
{
}

void Button_vc2013App::update()
{
}

void Button_vc2013App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( Button_vc2013App, RendererGl )
