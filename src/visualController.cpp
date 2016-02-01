#include "visualController.hpp"



void visualController::setup(){
    mCam.setup();
    mTracker.setup();
    mCam.setKeystone(mTracker.getKeystone());
}

void visualController::update(){
    mCam.update();
    mTracker.update();
    if(mTracker.newKeystone()){
        mCam.setKeystone(mTracker.getKeystone());
    }
}

void visualController::draw(){
    mCam.draw();
    mTracker.draw();
}

Color visualController::getColorOfMirror(int id){
    return mCam.getColorOfMirror(id);
}