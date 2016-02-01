#include "visualController.hpp"



void visualController::setup(){
    
    hue = 50;
    sat = 30;
    val = 20;
    
    mCam.setup();
    mImage.setup();
    mImage.getColorForMirrorId(0);
    mImage.getColorForMirrorId(29);
    mImage.getColorForMirrorId(899-29);
    mImage.getColorForMirrorId(899);
    colorDifference(0);
    mTracker.setup();
    mCam.setKeystone(mTracker.getKeystone());
    
    
}

void visualController::update(){
    mCam.update();
    mTracker.update();
    mImage.update();
    if(mTracker.newKeystone()){
        mCam.setKeystone(mTracker.getKeystone());
    }
    colorDifference(0);
}

void visualController::draw(){
    mCam.draw();
    mTracker.draw();
    mImage.draw();
}

Color visualController::getColorOfMirror(int id){
    return mCam.getColorOfMirror(id);
}

void visualController::colorDifference(int id){
    Color mirrorColor       = mCam.getColorOfMirror(id);
    Color imageColor        = mImage.getColorForMirrorId(id);
    vec3 vM                 = mirrorColor.get(CM_HSV);
    vec3 vI                 = imageColor.get(CM_HSV);
    
    vec3 vM2                = vec3(
                                   lmap(double(vM.x*hue),0.0,100.0,0.0,1.0),
                                   lmap(double(vM.y*sat),0.0,100.0,0.0,1.0),
                                   lmap(double(vM.z*val),0.0,100.0,0.0,1.0));
    vec3 vI2                = vec3(
                                   lmap(double(vI.x*hue),0.0,100.0,0.0,1.0),
                                   lmap(double(vI.y*sat),0.0,100.0,0.0,1.0),
                                   lmap(double(vI.z*val),0.0,100.0,0.0,1.0));
    Color mirrorColorHSV    = Color(CM_HSV,vM);
    Color imageColorHSV     = Color(CM_HSV,vI);
    Color mirrorColorHSV2   = Color(CM_HSV,vM2);
    Color imageColorHSV2    = Color(CM_HSV,vI2);
    float colorDistRGB      = distance(mirrorColor, imageColor);
    float colorDistHSV      = distance(mirrorColorHSV, imageColorHSV);
    float colorDistHSV2     = distance(mirrorColorHSV2, imageColorHSV2);
//    console() << "ID: " << id << " [RGB: " << colorDistRGB << "][HSV: " << colorDistHSV << "]" << endl;
    stringstream ss;
    ss << "ID: " << id << " [RGB: " << colorDistRGB << "][HSV: " << colorDistHSV << "][HSV2: " << colorDistHSV2 << "]";
    vconsole.print(ss.str());
    
    
    
}
