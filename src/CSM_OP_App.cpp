#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CSM_OP_App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void CSM_OP_App::setup()
{
}

void CSM_OP_App::mouseDown( MouseEvent event )
{
}

void CSM_OP_App::update()
{
}

void CSM_OP_App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( CSM_OP_App, RendererGl )
