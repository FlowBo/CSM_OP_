//
//  tinyG.cpp
//  CSM_Operator
//
//  Created by Florian Born on 13/12/15.
//
//

#include "tinyG.hpp"


tinyG::tinyG(){};

tinyG::tinyG(int enableTinyg){
    if (enableTinyg) {
        mUseTinyG = true;
    }else{
        mUseTinyG = false;
    }
};

void tinyG::setup(){
    mCounter            = 0;
    mLastRead           = 0;
    mLastUpdate         = 0;
    mStepSize           = 1.0;
    mSendSerialMessage  = false;
    mFeedhold           = false;
    mBusy               = false;
    
    // print the devices
    for(const auto &dev : Serial::getDevices()){
        console() << "Device: " << dev.getName() << endl;
    }
    
    try {
        Serial::Device dev = Serial::findDeviceByNameContains( "tty.usbserial" );
        mSerial = Serial::create( dev, SERIAL_PORT );
        console() << "created Device" << endl;
        mUseTinyG = true;
    }
    catch( SerialExc &exc ) {
        CI_LOG_EXCEPTION( "coult not initialize the serial device", exc );
        //        exit( -1 );
        mUseTinyG = false;
        console() << "runing software without TinyG" << endl;
    }
    if (mUseTinyG) {
        mSerial->flush();
    }
    
    
    loadModuleOffsets();
    
}

void tinyG::loadModuleOffsets(){
    try{
        mJson = JsonTree( app::loadAsset( "module_offsets.json" ));
        for (auto &module : mJson) {
            double x = module["xPos"].getValue<double>();
            double y = module["yPos"].getValue<double>();
            mModulPositions.clear();
            mModulPositions.push_back(dvec2(x,y));
        }
    }
    catch( ci::Exception &exc ) {
        console() << "Failed to parse json, what: " << exc.what() << std::endl;
    }
}

void tinyG::saveModuleOffsets(){
    fs::path localFile = getAssetPath("") / "module_offsets.json";
    JsonTree mNewJson;
    int id = 0;
    for (auto &module : mModulPositions) {
        JsonTree t;
        t.addChild( JsonTree( "id", id));
        t.addChild( JsonTree( "xPos", module.x));
        t.addChild( JsonTree( "yPos", module.y));
        id++;
    }
    mNewJson.write( localFile );
    console() << "saved new Offset to: " << localFile << endl;
}

void tinyG::update(){
    if(mUseTinyG){
        if( mMessage != "") {
            console() << "gCode: " << mMessage << endl;
            ostringstream gCode;
            gCode << mMessage << '\n';
            mSerial -> writeString(gCode.str());
            mMessage = "";
        }
        while (mSerial -> getNumBytesAvailable()) {
            try{
                mLastString = mSerial -> readStringUntil( '\n', BUFSIZE );
                parseJSON(mLastString);
            }
            catch( SerialTimeoutExc &exc ) {
                CI_LOG_EXCEPTION( "timeout", exc );
            }
        }
    }
}

void tinyG::sendGcode(string t){
    mMessage = t;
}

void tinyG::move(string s){
    sendGcode(s + toString(mStepSize));
}

void tinyG::draw(){
    
}

void tinyG::parseJSON( string input )
{
    console() << input << endl;
    try {
        JsonTree json(  input  );
        if (json.hasChild("sr")) {
            if(json["sr"].hasChild("posx")){
                xPos = json["sr"]["posx"].getValue<float>();
            }
            if(json["sr"].hasChild("posy")){
                yPos = json["sr"]["posy"].getValue<float>();
            }
            if(json["sr"].hasChild("posz")){
                zPos = json["sr"]["posz"].getValue<float>();
            }
            if(json["sr"].hasChild("posa")){
                aPos = json["sr"]["posa"].getValue<float>();
            }
            if(json["sr"].hasChild("stat")){
                stat = json["sr"]["stat"].getValue<int>();
            }
        }else{
        }
    }
    catch( ci::Exception &exc ) {
        console() << "Failed to parse json, what: " << exc.what() << std::endl;
    }
}


void tinyG::setModuleOffest(int id){
    if(mModulPositions.size() < id){
        console() << "entered id over 24 or didn't loaded old positions!";
        return;
    }
    mModulPositions.at(id).x = xPos;
    mModulPositions.at(id).y = yPos;
    console() << "module [" << id << "] new Offset Position is [" << mModulPositions.at(id) << "]" << endl;
    saveModuleOffsets();
}







