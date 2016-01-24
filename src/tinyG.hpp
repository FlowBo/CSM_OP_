#pragma once

#include "cinder/app/App.h"
#include "cinder/Serial.h"
#include "cinder/Log.h"
#include "cinder/Text.h"
#include "cinder/Json.h"
#include "cinder/app/RendererGl.h"

#include <sstream>

using namespace std;
using namespace ci;
using namespace ci::app;

#define BUFSIZE 10000
#define READ_INTERVAL 250
#define SERIAL_PORT 115200


class tinyG{
public:
    tinyG();
    void setup();
    void update();
    void sendGcode(std::string t );
    void draw();
    void parseJSON( string input );
    void move(string s);
    
    bool		mSendSerialMessage;
    SerialRef	mSerial;
    uint8_t		mCounter;
    string		mLastString;
    string      mMessage;
    bool        mFeedhold;
    double      mLastRead, mLastUpdate;
    float       mStepSize;
    
    //Getter Functions
    bool    isBusy() {return mBusy;};
    bool    feedhold() {return mFeedhold;};
    float   getXPos() {return xPos;};
    float   getYPos() {return yPos;};
    float   getZPos() {return zPos;};
    float   getAPos() {return aPos;};
    int     getStat() {return stat;};
    
    //setter functions
    void setFeedhold(bool b){mFeedhold = b;};
    void setStepSize(float size){mStepSize = size;};
    
private:
    bool    mBusy;
    int     statusInterval = READ_INTERVAL;
    
    //TinyG status
    float xPos  = 0;
    float yPos  = 0;
    float zPos  = 0;
    float aPos  = 0;
    int stat    = 3;
    int lastStat;
    
};