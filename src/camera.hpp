#include "cinder/Capture.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class camera{
public:
    camera(){};
    void setup();
    void update();
    void draw();
    
private:
    CaptureRef			mCapture;
    gl::TextureRef		mTexture;
};