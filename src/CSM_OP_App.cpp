#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/cairo/Cairo.h"
#include "oscControll.hpp"
#include "tinyG.hpp"



using namespace ci;
using namespace ci::app;
using namespace std;

class CSM_OP_App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    void keyDown(KeyEvent event) override;
    
    tinyG           tiny;
    oscControll     osc;
    
    float   mFramerate;
    Font    mFont;
};

void CSM_OP_App::setup()
{
    tiny.setup();
    tiny.sendGcode( "g91" );
    osc.setup( tiny );
    mFont = Font("Helvetica",14);
}

void CSM_OP_App::mouseDown( MouseEvent event )
{
}

void CSM_OP_App::update()
{
    osc.update();
    tiny.update();
}

void CSM_OP_App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}
void CSM_OP_App::keyDown(KeyEvent event){
    if (event.getChar() == ' ') {
    }
}

CINDER_APP( CSM_OP_App, RendererGl )
