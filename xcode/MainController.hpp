#pragma once
#include "cinder/app/App.h"
#include "tinyG.hpp"
#include "oscControll.hpp"
#include "visualController.hpp"
#include "vConsole.hpp"
#include "cinder/Json.h"

using namespace ci;
using namespace ci::app;
using namespace std;

enum cycle{
    HOME,
    SCREW_HOME,
    ENGAGING_SCREW,
    DISENGAGING_SCREW,
    SCAN_COLORS,
    GOTO_COLOR,
    TRAVERSE,
    REST,
    NEW_MIRROR,
    GOTO_ZERO_A
};

class MainController{
public:
    MainController(){};
    void setup();
    void update();
    void draw();
    void setModuleOffset(int id);
private:
    
    vector<dvec2> mMirrorPosition;
    vector<dvec2> mModuleOffsets;
    int currentMirrorId;
    
    void loadOffset();
    void saveCurrentOffset();
    void calcMirrorPosPerModul();
    
    tinyG               tiny;
    oscControll         osc;
    visualController    vc;
    
    void run();
//    void engageScrewCycle();
//    void disengageScrewCycle();
//    void screwHomeCycle();
//    void gotoZeroA();
//    void goToCloasestColor();
//    void updateCloasestColor();
    
    double screwGap = 33.8214;
    
    bool dryRun;
    bool startMachine;
    enum cycle cycle;
    enum cycle lastCycle;
};