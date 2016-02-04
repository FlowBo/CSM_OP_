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
    mWaitForStatus      = false;
    
    // print the devices
    for(const auto &dev : Serial::getDevices()){
        console() << "Device: " << dev.getName() << endl;
    }
    try {
        Serial::Device dev = Serial::findDeviceByNameContains( "tty.usbserial" );
        mSerial = Serial::create( dev, SERIAL_PORT );
        console() << "created Device" << endl;
        mUseTinyG = true;
        sendGcode("{ej:1}");
    }
    catch( SerialExc &exc ) {
        CI_LOG_EXCEPTION( "coult not initialize the serial device", exc );
        //        exit( -1 );
        mUseTinyG = false;
        console() << "runing software without TinyG" << endl;
    }
    if (mUseTinyG) {
        mSerial->flush();
        getNewStatusreport();
    }
}

void tinyG::loadModuleOffsets(){
    try{
        mJson = JsonTree( app::loadAsset( "module_offsets.json" ));
        mModulPositions.clear();
        for (auto &module : mJson) {
            double id = module["id"].getValue<int>();
            double x = module["xPos"].getValue<double>();
            double y = module["yPos"].getValue<double>();
            dvec2 position = dvec2(x,y);
            mModulPositions.push_back(position);
            std::stringstream ss;
            if(id < 10){
                ss << "id_0" << id << " - " << position;
            }else{
                ss << "id_" << id << " - " << position;
            }
            vconsole.print(ss.str());
            value = 0;
        }
    }
    catch( ci::Exception &exc ) {
        console() << "Failed to parse json, what: " << exc.what() << std::endl;
    }
    
}

void tinyG::update(){
    if(mUseTinyG){
        if( mSerialMessages.size() != 0) {
            console() << "gCode: " << mMessage << endl;
            ostringstream gCode;
            gCode << mSerialMessages.at(0) << '\n';
            mSerial -> writeString(gCode.str());
            mSerialMessages.erase(mSerialMessages.begin());
        }
        if(mSerial -> getNumBytesAvailable() && mWaitForStatus){
            vconsole.print("Status Report");
            vconsole.print("-------------");
        }
        while (mSerial -> getNumBytesAvailable()) {
            try{
                if(mWaitForStatus){
                    mLastString = mSerial -> readStringUntil( '\n', BUFSIZE );
                    //                    reformatStatusReport(mLastString);
                    parseJSON(mLastString);
                    stringstream ss;
                    ss << "x: " << xPos;
                    vconsole.print(ss.str());
                    ss.str("");
                    ss << "y: " << yPos;
                    vconsole.print(ss.str());
                    ss.str("");
                    ss << "z: " << zPos;
                    vconsole.print(ss.str());
                    ss.str("");
                    ss << "a: " << aPos;
                    vconsole.print(ss.str());
                    ss.str("");
                    ss << "stat: " << stat;
                    vconsole.print(ss.str());
                    vconsole.print("-------------");
                    waitForStatusReport(false);
                }else{
                    mLastString = mSerial -> readStringUntil( '\n', BUFSIZE );
                    parseJSON(mLastString);
                }
                
                
            }
            catch( SerialTimeoutExc &exc ) {
                CI_LOG_EXCEPTION( "timeout", exc );
            }
        }
    }
    //    if(stat == 3 && mBusy){
    //        mBusy = false;
    //    }
    //    vconsole.print(to_string(stat));
}

void tinyG::getNewStatusreport(){
    waitForStatusReport(true);
    sendGcode("{sr:{posx:t,posy:t,posz:t,posa:t,stat:t}}");
}


void tinyG::sendGcode(string t){
    mMessage = t;
    mSerialMessages.push_back(t);
    mBusy = true;
}

void tinyG::move(string s){
    sendGcode("g91");
    sendGcode(s + toString(mStepSize));
    sendGcode("g90");
}

void tinyG::gotoModule(int id){
    if(mModulPositions.size() < id){
        console() << "entered id over 24 or didn't loaded old positions!";
        return;
    }
    dvec2 v = mModulPositions.at(id);
    stringstream ss;
    ss << "g0x"<< v.x << "y" << v.y;
    vconsole.print(ss.str());
    sendGcode(ss.str());
}

void tinyG::draw(){
    
}

void tinyG::waitForStatusReport(bool b)
{
    mWaitForStatus = b;
}

void tinyG::parseJSON( string input )
{
    
//    console() << "input: " << input << endl;
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
                lastStat = stat;
                stat = json["sr"]["stat"].getValue<int>();
                if (stat == 3) {
                    mBusy = false;
                    vconsole.print("finished last command");
                }
            }
        }else if (json.hasChild("r")) {
            if(json["r"].hasChild("sr")){
                JsonTree json2 = json["r"].getChild("sr");
                if(json2.hasChild("posx")){
                    xPos = json2["posx"].getValue<float>();
                }
                if(json2.hasChild("posy")){
                    yPos = json2["posy"].getValue<float>();
                }
                if(json2.hasChild("posz")){
                    zPos = json2["posz"].getValue<float>();
                }
                if(json2.hasChild("posa")){
                    aPos = json2["posa"].getValue<float>();
                }
                if(json2.hasChild("stat")){
                    stat = json2["stat"].getValue<int>();
                    if (stat == 3) {
                        mBusy = false;
                        vconsole.print("finished last command");
                    }
                }
            }else{
                getNewStatusreport();
            }
        }else{};
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

void tinyG::saveModuleOffsets(){
    fs::path localFile = getAssetPath("") / "module_offsets.json";
    JsonTree mNewJson;
    int id = 0;
    for (auto &module : mModulPositions) {
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

void tinyG::setBusy(){
    mBusy = true;
}

bool tinyG::isBusy(){
    if(mBusy){
        return true;
    }
    return false;
};



