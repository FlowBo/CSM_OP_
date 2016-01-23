//
//  tinyG.cpp
//  CSM_Operator
//
//  Created by Florian Born on 13/12/15.
//
//

#include "tinyG.hpp"


tinyG::tinyG(){};

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
    }
    catch( SerialExc &exc ) {
        CI_LOG_EXCEPTION( "coult not initialize the serial device", exc );
        exit( -1 );
    }
    mSerial->flush();
}


void tinyG::update(){
    
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
