#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "GUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class Img{
public:
    Img(){};
    
    void setup();
    void update();
    void draw();
    Color getColorForMirrorId(int id);
    
private:
    Surface             mSurface;
    gl::TextureRef     mImage;
};