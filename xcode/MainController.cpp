#include "MainController.hpp"


void MainController::setup(){
    dryRun          = false;
    notScanning     = true;
    startMachine    = false;
    cycle           = HOME;
    nextCycle       = HOME;
    nextXPos        = 0.0;
    nextYPos        = 0.0;
    nextZPos        = 0.0;
    nextAPos        = 0.0;
    loadMainOffset();
    loadOffset();
    calcMirrorPosPerModul();
    tiny.setup();
    osc.setup(tiny);
    vc.setup();
    
    
}

void MainController::update(){
    run();
    tiny.update();
    osc.update();
    
    if(osc.hasNewMainOffset()){
        setMainOffset();
    }
    if(osc.hasNewOffset()){
        setModuleOffset(osc.getNewOffsetId());
    }
    if(osc.hasNewGoTo()){
        int id = osc.getNewGoToId();
        dvec2 v = mModuleOffsets.at(id);
        tiny.sendGcode("g0z0");
        stringstream ss;
        ss << "g0x" << v.x << "y" << v.y;
        tiny.sendGcode(ss.str());
    }
    vc.update();
}

void MainController::draw(){
    vc.draw();
}

void MainController::loadMainOffset(){
    JsonTree mJson;
    try{
        mJson = JsonTree( app::loadAsset( "main_Offset.json" ));
        mMainOffset.x = mJson["xPos"].getValue<double>();
        mMainOffset.y = mJson["yPos"].getValue<double>();
        stringstream ss;
        ss << "G10L2P1X" << mMainOffset.x << "Y" << mMainOffset.y;
        tiny.sendGcode(ss.str());
        //        tiny.sendGcode("G54");
        
    }
    catch( ci::Exception &exc ) {
        console() << "Failed to parse json, what: " << exc.what() << std::endl;
    }
}

void MainController::loadOffset(){
    JsonTree mJson;
    try{
        mJson = JsonTree( app::loadAsset( "module_offsets.json" ));
        mModuleOffsets.clear();
        for (auto &module : mJson) {
            double id = module["id"].getValue<int>();
            double x = module["xPos"].getValue<double>();
            double y = module["yPos"].getValue<double>();
            dvec2 position = dvec2(x,y);
            mModuleOffsets.push_back(position);
            std::stringstream ss;
            if(id < 10){
                ss << "id_0" << id << " - " << position;
            }else{
                ss << "id_" << id << " - " << position;
            }
            vconsole.print(ss.str());
            //            value = 0;
        }
    }
    catch( ci::Exception &exc ) {
        console() << "Failed to parse json, what: " << exc.what() << std::endl;
    }
}

void MainController::setModuleOffset(int id){
    if(mModuleOffsets.size() < id){
        console() << "entered id [" << id << "] over 24 or didn't loaded old positions!";
        return;
    }
    mModuleOffsets.at(id).x = tiny.getXPos();
    mModuleOffsets.at(id).y = tiny.getYPos();
    console() << "module [" << id << "] new Offset Position is [" << mModuleOffsets.at(id) << "]" << endl;
    saveCurrentOffset();
}

void MainController::saveCurrentOffset(){
    fs::path localFile = getAssetPath("") / "module_offsets.json";
    JsonTree mNewJson;
    int id = 0;
    for (auto &module : mModuleOffsets) {
        JsonTree t;
        t.addChild( JsonTree( "id", id));
        t.addChild( JsonTree( "xPos", module.x));
        t.addChild( JsonTree( "yPos", module.y));
        mNewJson.addChild(t);
        id++;
    }
    console() << mNewJson << endl;
    mNewJson.write( localFile );
    console() << "saved new Offset to: " << localFile << endl;
}

void MainController::calcMirrorPosPerModul()
{
    vector<dvec2> tempPos;
    mMirrorPosition.clear();
    for(int i = 0 ; i < mModuleOffsets.size(); i++)
    {
        dvec2 origin = mModuleOffsets.at(i);
        for (int y = 0; y < 6; y++) {
            for(int x = 0; x < 6; x++){
                tempPos.push_back(dvec2(origin.x + (x * SCREW_GAP),origin.y + (y * SCREW_GAP)));
            }
        }
        if((i+1)%5 == 0){
            int row = 0;
            for (int y = 0; y < 6; y++)//6 rows
            {
                int skip = 0;
                for(int m = 0; m < 5; m++)// 5 Modul
                {
                    for (int x = 0; x < 6; x++) // get 6 Mirrors
                    {
                        mMirrorPosition.push_back(tempPos.at(row+x+skip));
                    }
                    skip += 36;//36 mirrors per modul
                }
                row += 6;
            }
            tempPos.clear();
        }
    }
    
    //PRINT ALL MIRROR POSITIONS
    
    //    int c = 0;
    //    for(vector<dvec2>::iterator iter = mMirrorPosition.begin(); iter != mMirrorPosition.end();++iter)
    //    {
    //        console() << mMirrorPosition.at(c) << endl;
    //        if ((c+1) % 30 == 0) {
    //            console() << "---------" << endl;
    //        }
    //        c++;
    //    }
    //
}

void MainController::run(){
    if(!tiny.isBusy() && startMachine){
        cycle = nextCycle;
        switch (cycle) {
            case HOME:
            {
                vconsole.print("homing maschine");
                tiny.sendGcode("g28.2x0y0z0");
                stringstream ss;
                ss << "G10L2P1X" << mMainOffset.x << "Y" << mMainOffset.y;
                tiny.sendGcode(ss.str());
                tiny.sendGcode("g0x0y0");
                tiny.sendGcode("g28.3a0");
                nextXPos = 0.0;
                nextYPos = 0.0;
                nextZPos = 0.0;
                nextAPos = 0.0;
                lastCycle = cycle;
                nextCycle = GOTO_MIRROR;
                break;
            }
            case REST:
                if(!checkPositions()){
                     cycle = lastCycle;//redo last Cycle
                    break;
                }
                vconsole.print("maschine is resting");
                break;
            case GOTO_MIRROR:
            {
                if(!checkPositions()){
                    cycle = lastCycle;//redo last Cycle
                    break;
                }
                getNextMirror();
                dvec2 v = mMirrorPosition.at(mCurrentMirrorId);
                stringstream s;
                s << "Next Mirror -> " << mCurrentMirrorId << v;
                vconsole.print(s.str());
                stringstream ss;
                ss << "g0x" << v.x << "y" << v.y;
                tiny.sendGcode(ss.str());
                nextXPos = v.x;
                nextYPos = v.y;
                lastCycle = cycle;
                nextCycle = ENGAGING_SCREW;
                break;
            }
            case ENGAGING_SCREW:
                if(!checkPositions()){
                    cycle = lastCycle;//redo last Cycle
                    break;
                }
                vconsole.print("engaging Screw");
                engageScrewCycle();
                
                lastCycle = cycle;
                if (dryRun) {
                    nextCycle = DISENGAGING_SCREW;
                }else{
                    nextCycle = SCREW_HOME;
                }
                vconsole.print("Next Cycle: SCREW_HOME");
                
                break;
            case SCREW_HOME:
               
                if(!checkPositions()){
                    nextCycle = lastCycle;//redo last Cycle
                    break;
                }
                
                tiny.sendGcode("g28.2a0");
                
                nextAPos = 0.0;
                lastCycle = cycle;
                nextCycle = SCAN_COLORS;
                break;
            case SCAN_COLORS:
                if(!checkPositions()){
                    cycle = lastCycle;//redo last Cycle
                    break;
                }
                scanColorCycle();
                //                lastCycle = cycle;
                break;
            case EVALUATE_COLORS:
                evaluateColors();
                lastCycle = cycle;
                nextCycle = DISENGAGING_SCREW;
                break;
            case DISENGAGING_SCREW:
                vconsole.print("disengaging Screw");
                tiny.sendGcode("g0z0");
                tiny.sendGcode("g28.3a0");//sets A-Axis to Zero
                nextZPos = 0.0;
                nextAPos = 0.0;
                lastCycle = cycle;
                nextCycle = GOTO_MIRROR;
                
                break;
            default:
                break;
        }
    }
}
void MainController::getNextMirror(){
    mCurrentMirrorId = random.nextInt(0,5);
    //    mCurrentMirrorId = 4;
}

void MainController::engageScrewCycle(){
    if(dryRun){
        tiny.sendGcode("g1z6.6a180f400");
        nextZPos = 6.5;
        nextAPos = 180.0;
    }else{
        stringstream ss;
        ss << "g1z11.8a180f400";
        tiny.sendGcode(ss.str());
        nextZPos = 11.8;
        nextAPos = 180.0;
    }
    
}

void MainController::evaluateColors(){
    if (!notScanning) {
        double cloasestDistance = 9999;
        Mirror mirror;
        for(auto &m : mCurrentMirrorColors){
            double currentDifference = m.getDifference();
            if(currentDifference < cloasestDistance){
                cloasestDistance = currentDifference;
                mirror = m;
            }
        }
        stringstream ss;
        ss << "g1a" << mirror.getRotationalPosition() << "f600";
        tiny.sendGcode(ss.str());
        nextAPos = mirror.getRotationalPosition();
    }
}

void MainController::scanColorCycle(){
    if(mCurrentMirrorColors.size() != 0){
        if (mCurrentMirrorColors.at(0).getId() != mCurrentMirrorId) {
            mCurrentMirrorColors.clear();
        }
    }
    if (mCurrentMirrorRotation < MAX_ROTATION) {
        if(!notScanning){
            Color mirrorColor = vc.getColorOfMirror(mCurrentMirrorId);
            double difference = vc.getCurrentColorDifference(mCurrentMirrorId);
            Mirror m;
            m.setId(mCurrentMirrorId);
            m.setRotationalPosition(mCurrentMirrorRotation);
            m.setColor(mirrorColor);
            m.setDiffence(difference);
            mCurrentMirrorColors.push_back(m);
        }
        mCurrentMirrorRotation += ANGLE_STEPS;
        stringstream ss;
        ss << "g1a" << mCurrentMirrorRotation << "f4200";
        tiny.sendGcode(ss.str());
        nextAPos = mCurrentMirrorRotation;
    }else{
        mCurrentMirrorRotation = 0;
        lastCycle = cycle;
        nextCycle = EVALUATE_COLORS;
    }
    
}

bool MainController::checkPositions(){
    if(tiny.getXPos() != nextXPos){
        stringstream ss;
        ss << "Tiny X: " << tiny.getXPos() << " Next X: " << nextXPos;
        vconsole.print(ss.str());
        return false;
    }
    if(tiny.getYPos() != nextYPos){
        stringstream ss;
        ss << "Tiny Y: " << tiny.getYPos() << " Next Y: " << nextYPos;
        vconsole.print(ss.str());
        return false;
    }
    if(!areEqual(tiny.getYPos(), nextYPos)){
        vconsole.print("Z not equal");
        return false;
    }
    if(tiny.getAPos() != nextAPos){
        stringstream ss;
        ss << "Tiny A: " << tiny.getAPos() << " Next A: " << nextAPos;
        vconsole.print(ss.str());
        return false;
    }
    return true;
}

void MainController::setMainOffset(){
    dvec2 v = dvec2(tiny.getXPos() + mMainOffset.x ,tiny.getYPos() + + mMainOffset.y);
    mMainOffset = v;
    stringstream ss;
    ss << "G10L2P1X" << mMainOffset.x << "Y" << mMainOffset.y;
    tiny.sendGcode(ss.str());
    tiny.sendGcode("G54");
    try{
        fs::path localFile = getAssetPath("") / "main_offset.json";
        JsonTree mNewJson;
        mNewJson.addChild( JsonTree( "xPos", mMainOffset.x));
        mNewJson.addChild( JsonTree( "yPos", mMainOffset.y));
        console() << mNewJson << endl;
        mNewJson.write( localFile );
        console() << "saved new Main Offset to: " << localFile << endl;
    }
    catch( ci::Exception &exc ) {
        console() << "Failed to parse json, what: " << exc.what() << std::endl;
    }
}

void MainController::recalculateOffsets(){
    for(auto &o : mModuleOffsets){
        double xDiff = mMainOffset.x - o.x;
        double yDiff = mMainOffset.y - o.y;
        double newX = o.x + xDiff;
        double newY = o.y + yDiff;
        o.x = newX;
        o.y = newY;
    }
    saveCurrentOffset();
}

void MainController::toggleStart(){
    startMachine = !startMachine;
}

bool MainController::areEqual(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

