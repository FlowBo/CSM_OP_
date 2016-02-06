#pragma once
#include "cinder/app/App.h"
#include "tinyG.hpp"
#include "oscControll.hpp"
#include "visualController.hpp"
#include "vConsole.hpp"
#include "cinder/Json.h"
#include "cinder/Rand.h"
#include "Mirror.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

#define ANGLE_STEPS 30
#define MAX_ROTATION 3980
#define SCREW_GAP 16.75
#define Z_SCREW_DIST 11.8


enum cycle{
    HOME,
    SCREW_HOME,
    ENGAGING_SCREW,
    DISENGAGING_SCREW,
    SCAN_COLORS,
    GOTO_COLOR,
    GOTO_MIRROR,
    REST,
    EVALUATE_COLORS
};

class MainController{
public:
    MainController(){};
    void setup();
    void update();
    void draw();
    void setModuleOffset(int id);
    void setModuleOffsetTR(int id);
    void toggleStart();
    
private:
    
    vector<dvec2> mMirrorPosition;
    vector<dvec2> mModuleOffsets;
    vector<dvec2> mModuleOffsetsTR;
    vector<Mirror> mCurrentMirrorColors;
    
    dvec2 rotateVector(dvec2 v , float degree);
    
    int mCurrentMirrorId;
    double mCurrentMirrorRotation;
    dvec2 mMainOffset;
    
    dvec2 interpolateVec(dvec2 vec, dvec2 ORIGIN, dvec2 BR,dvec2 TL,dvec2 TR);
    
    void recalculateOffsets();
    void setMainOffset();
    void loadOffset();
    void loadMainOffset();
    void saveCurrentOffset();
    void calcMirrorPosPerModul();
    bool checkPositions();
    void getNextMirror();
    
    bool areEqual(double a, double b);
    
    tinyG               tiny;
    oscControll         osc;
    visualController    vc;
    
    void run();
    void engageScrewCycle();
    void scanColorCycle();
    void evaluateColors();
//    void disengageScrewCycle();
//    void screwHomeCycle();
//    void gotoZeroA();
//    void goToCloasestColor();
//    void updateCloasestColor();
    
//    double screwGap = 33.8214;
    
    bool dryRun;
    bool notScanning;
    bool startMachine;
    enum cycle cycle;
    enum cycle lastCycle;
    enum cycle nextCycle;
    
    
    double nextXPos;
    double nextYPos;
    double nextZPos;
    double nextAPos;
    Rand random;

};