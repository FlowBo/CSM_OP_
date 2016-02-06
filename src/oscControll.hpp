
//
//  oscControll.hpp
//  CSM_Operator
//
//  Created by Florian Born on 22.01.16.
//
//
#pragma once
#include "Osc.h"
#include "tinyG.hpp"

#define USE_UDP 1
#define PORT 12000

using namespace ci;
using namespace ci::app;
using namespace std;


class oscControll{
public:
    oscControll() : mReceiver( PORT ){};
    void    setup( tinyG &t );
    void    update();
    float   stepSize;
    bool    hasNewOffset(){return newOffset;};
    bool    hasNewOffsetTR(){return newOffsetTR;};
    bool    hasNewGoTo(){return mNewGoTo;};
    bool    hasNewMainOffset();
    int     getNewOffsetId();
    int     getNewOffsetIdTR();
    int     getNewGoToId();
    
private:
#if USE_UDP
    osc::ReceiverUdp mReceiver;
#else
    osc::ReceiverTcp mReceiver;
#endif
    
    tinyG   *tiny;
    bool    newOffset;
    bool    newOffsetTR;
    bool    mNewGoTo;
    bool    mNewMainOffset;
    int     newOffsetId;
    int     newOffsetIdTR;
    int     mNewGoToId;
};