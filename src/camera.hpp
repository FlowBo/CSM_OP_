#include "cinder/Capture.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "tracking.hpp"

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
    void        calculateColors();
    void        drawRealWorldColors();
    
    vec2                interpolateVec(dvec2 vec);
    Color               findSurroundingColors( vec2 vec, float searchDiameter);
    tracking            tracker;
    CaptureRef			mCapture;
    gl::TextureRef		mTexture;
    Surface8uRef        mSurface;
    
    //Keystone
    dvec2 mTopLeft;
    dvec2 mTopRight;
    dvec2 mBottomLeft;
    dvec2 mBottomRight;
    
    // Color
    std::vector<Color>  mRealWorldColors;
    std::vector<vec2>   mMirrorVirtualPosition;
    std::vector<vec2>   mMirrorPositionInterpolated;

};