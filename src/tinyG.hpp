#pragma once
//#include "CSM_OP_App.h"
#include "cinder/app/App.h"
#include "cinder/Serial.h"
#include "cinder/Log.h"
#include "cinder/Text.h"
#include "cinder/Json.h"
#include "cinder/app/RendererGl.h"
#include "GUI.h"

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
    tinyG(int enableTinyg);
    void setup();
    void update();
    void sendGcode(std::string t );
    void draw();
    void parseJSON( string input );
    void move(string s);
    void gotoModule(int id);
    void loadModuleOffsets();
    void saveModuleOffsets();
    
    
    bool		mSendSerialMessage;
    SerialRef	mSerial;
    uint8_t		mCounter;
    string		mLastString;
    string      mMessage;
    bool        mFeedhold;
    double      mLastRead, mLastUpdate;
    float       mStepSize;
    JsonTree    mJson;
    bool        mUseTinyG;
    bool        mWaitForStatus;

    
    vector<string> mSerialMessages;
    
    void getNewStatusreport();
    
    //Getter Functions
    bool    isBusy();
    void    setBusy();
    bool    feedhold() {return mFeedhold;};
    float   getXPos() {return xPos;};
    float   getYPos() {return yPos;};
    float   getZPos() {return zPos;};
    float   getAPos() {return aPos;};
    int     getStat() {return stat;};
    
    //setter functions
    void setFeedhold(bool b){mFeedhold = b;};
    void setStepSize(float size){mStepSize = size;};
    void setModuleOffest(int id);
    void waitForStatusReport(bool b);
    
    
private:
    bool    mBusy;
    int     statusInterval = READ_INTERVAL;
    
    //TinyG status
    double   xPos  = 0;
    double   yPos  = 0;
    double   zPos  = 0;
    double   aPos  = 0;
    int     stat  = 3;
    int     lastStat;
    
    vector<dvec2> mModulPositions;
    
};





