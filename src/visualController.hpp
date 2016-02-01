#pragma once
#include "camera.hpp"
#include "tracking.hpp"

class visualController{
public:
    visualController(){};
    void setup();
    void update();
    void draw();
    
    Color           getColorOfMirror(int id);
    Surface8uRef    getCameraImage(){return mCam.getCameraImage();};
    
private:
    camera      mCam;
    tracking    mTracker;
};