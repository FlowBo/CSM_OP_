#include "MainController.hpp"


void MainController::setup(){
    dryRun = true;
    startMachine = false;
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
    if(osc.hasNewOffset()){
        setModuleOffset(osc.getNewOffsetId());
    }
    vc.update();
}

void MainController::draw(){
    vc.draw();
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
        console() << "entered id over 24 or didn't loaded old positions!";
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
                tempPos.push_back(dvec2(origin.x + (x * screwGap),origin.y + (y * screwGap)));
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


}
