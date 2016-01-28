#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/cairo/Cairo.h"
#include "oscControll.hpp"
#include "tinyG.hpp"
#include "visualController.hpp"
#include "vConsole.hpp"
#include <iostream>


#define ENABLE_TINYG 1
#define CAMERA 1

vConsole vconsole;

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
    
    
    tinyG               tiny;
    oscControll         osc;
    visualController    vc;
    
    float   mFramerate;
    Font    mFont;
};

void CSM_OP_App::setup()
{
    vconsole.setup();
    tiny.setup();
    tiny.sendGcode( "g91" );
    osc.setup( tiny );
    vc.setup();
    mFont = Font("Helvetica",14);
    vconsole.print("hallo");
    
    
}

void CSM_OP_App::mouseDown( MouseEvent event )
{
}

void CSM_OP_App::update()
{
    vc.update();
    osc.update();
    tiny.update();
    mFramerate = getAverageFps();
    vconsole.update();
}

void CSM_OP_App::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    vc.draw();
    gl::drawString(toString(mFramerate), vec2(10,10));
    vconsole.draw();
    
}
void CSM_OP_App::keyDown(KeyEvent event){
    if (event.getChar() == KeyEvent::KEY_ESCAPE) {
        quit();
    }
    if (event.getChar() == KeyEvent::KEY_SPACE) {
        vconsole.print(to_string(random()*10));
        writeImage(getAppPath() / "images" / "saveImage_" / ( toString( random()*10 ) + ".jpg" ), *vc.getCameraImage() );
    }
    if (event.getChar() == KeyEvent::KEY_a ){
        unsigned year = 974;
        
        
        
        // Save it as text
        
        ofstream outtxt(getAppPath().string() + "textual.txt" );
        for(int i = 0; i < 1000; i++){
            outtxt << toString( random()*10) << "\n" << flush;
        }
        
        outtxt.close();
        
        // Save it as binary
        ofstream outbin( getAppPath().string() + "binary.bin", ios::binary );
        
        for(int i = 0; i < 1000; i++){
            string s = toString( random()*10) + "\n";
            outbin.write( reinterpret_cast <const char*> (&s), sizeof( s ) );
        }
        
//        outbin.write( reinterpret_cast <const char*> (&year), sizeof( year ) );
        outbin.close();
    }
}

CINDER_APP( CSM_OP_App, RendererGl(), [&]( App::Settings *settings ){
    settings -> setWindowSize(1280,720);
    settings -> setFrameRate(60);
    settings -> setTitle("CSM APP");
})
