#pragma once
#include "cinder/app/App.h"
#include "camera.hpp"
#include "tracking.hpp"
#include "Img.hpp"
#include "GUI.h"
using namespace ci;
using namespace ci::app;
using namespace std;


class visualController{
public:
    visualController(){};
    void setup();
    void update();
    void draw();
    
    Color           getColorOfMirror(int id);
    double          getCurrentColorDifference(int id);
    Surface8uRef    getCameraImage(){return mCam.getCameraImage();};
    
private:
    
    float hue;
    float sat;
    float val;
    camera      mCam;
    tracking    mTracker;
    Img         mImage;
    
};