#include "CSM_OP_App.h"


void CSM_OP_App::setup()
{
    roundTest = (int)10.92312312;
    console() << roundTest << endl;
    vconsole.setup();
    controller.setup();
    value = 10;
    tiny.setup();
    osc.setup( tiny );
    vc.setup();
    mFont = Font("Helvetica",14);
}

void CSM_OP_App::mouseDown( MouseEvent event )
{
}

void CSM_OP_App::update()
{
//    vc.update();
//    osc.update();
//    tiny.update();
    controller.update();
    mFramerate = getAverageFps();
    vconsole.update();
    
}

void CSM_OP_App::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    controller.draw();
//    vc.draw();
    gl::drawString(toString(mFramerate), vec2(10,10));
    vconsole.draw();
    
}
void CSM_OP_App::keyDown(KeyEvent event){
    if (event.getChar() == KeyEvent::KEY_ESCAPE) {
        quit();
    }
    if (event.getChar() == KeyEvent::KEY_SPACE) {
        vconsole.print(to_string(random()*10));
//        writeImage(getAppPath() / "images" / "saveImage_" / ( toString( random()*10 ) + ".jpg" ), *vc.getCameraImage() );
    }
    if (event.getChar() == KeyEvent::KEY_a ){
        ofstream outtxt(getAppPath().string() + "textual.txt" );
        for(int i = 0; i < 1000; i++){
            outtxt << toString( random()*10) << "\n" << flush;
        }
        outtxt.close();
        }
}

CINDER_APP( CSM_OP_App, RendererGl(), [&]( App::Settings *settings ){
    settings -> setWindowSize(1280,720);
    settings -> setFrameRate(60);
    settings -> setTitle("CSM APP");
})
