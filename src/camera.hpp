#pragma once
#include "cinder/Capture.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "tracking.hpp"
#include "GUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class camera{
public:
    camera(){};
    void setup();
    void update();
    void draw();
    void setKeystone(vector<dvec2> v);
    Color getColorOfMirror(int id);
    Surface8uRef getCameraImage(){return mSurface;};
private:
    void        calculateColors();
    void        drawRealWorldColors();
    
    vec2                interpolateVec(dvec2 vec);
    Color               findSurroundingColors( vec2 vec, float searchDiameter);
    
    void createMirrorScreenPosition();
    void calculateMirrorPositions();
    CaptureRef			mCapture;
    gl::TextureRef		mTexture;
    Surface8uRef        mSurface;
    bool                mUseCamera;
    
    //Keystone
    
    dvec2 mTopLeft;
    dvec2 mTopRight;
    dvec2 mBottomLeft;
    dvec2 mBottomRight;
    
    dvec2 mTLsurface;
    dvec2 mTRsurface;
    dvec2 mBLsurface;
    dvec2 mBRsurface;
    
    double mInstallationWidth;
    double mMirrorSize;
    
    // Color
    std::vector<Color>  mRealWorldColors;
    std::vector<vec2>   mMirrorVirtualPosition;
    std::vector<vec2>   mMirrorPositionInterpolated;
    std::vector<dvec2>  mMirrorPositionScreenPosition;

};