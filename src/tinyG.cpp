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
                    reformatStatusReport(mLastString);
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
}

bool tinyG::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void tinyG::reformatStatusReport(string &s){
    replace(mLastString, "mm", "");
    replace(mLastString, "deg", "");
    replace(mLastString, "position", "");
    mLastString.erase(std::remove(mLastString.begin(), mLastString.end(), ' '), mLastString.end());
    mLastString.erase(std::remove(mLastString.begin(), mLastString.end(), '\n'), mLastString.end());
    vector<string> strings = split(mLastString,':');
    if(strings.at(0) == "X"){
        xPos =  stof(strings.at(1));
    }
    if(strings.at(0) == "Y"){
        yPos =  stof(strings.at(1));
    }
    if(strings.at(0) == "Z"){
        zPos =  stof(strings.at(1));
    }
    if(strings.at(0) == "A"){
        aPos =  stof(strings.at(1));
    }
    if(strings.at(0) == "Machinestate"){
        sendGcode("{ej:1}");
        waitForStatusReport(false);
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
        vconsole.print("-------------");
    }
}


void tinyG::sendGcode(string t){
//    mMessage = t;
    mSerialMessages.push_back(t);
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
    console() << "input: " <<input << endl;
    
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






