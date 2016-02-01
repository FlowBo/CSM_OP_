//
//  oscControll.cpp
//  CSM_Operator
//
//  Created by Florian Born on 22.01.16.
//
//

#include "oscControll.hpp"

void oscControll::setup( tinyG &t ){
    newOffset   = false;
    newOffsetId = 99;
    console() << "OSC is listening to PORT: "  << PORT << endl;
    tiny = &t;
    mReceiver.setListener( "/move/x", [&] ( const osc::Message &msg ) {
        tiny -> setStepSize( msg[0].flt() );
        tiny -> move("g0x");
    });
    mReceiver.setListener( "/move/y", [&] ( const osc::Message &msg ) {
        tiny -> setStepSize( msg[0].flt() );
        tiny -> move("g0y");
    });
    mReceiver.setListener( "/move/z", [&] ( const osc::Message &msg ) {
        tiny -> setStepSize( msg[0].flt() );
        tiny -> move("g0z");
    });
    mReceiver.setListener( "/move/a", [&] ( const osc::Message &msg ) {
        tiny -> setStepSize( msg[0].flt() * 360.0 );
        tiny -> move("g0a");
    });
    mReceiver.setListener( "/info/statusReport", [&] ( const osc::Message &msg ) {
        tiny -> waitForStatusReport(true);
        tiny -> sendGcode("$sr");
    });
    mReceiver.setListener( "/move/goto", [&] ( const osc::Message &msg ) {
        tiny->gotoModule(std::atoi(msg[0].string().c_str()));
    });
    mReceiver.setListener( "/control/hold", [&] ( const osc::Message &msg ) {
        tiny->setFeedhold( !tiny->feedhold() );
        if (tiny->feedhold()) {
            tiny->sendGcode("!");
        }else{
            tiny->sendGcode("~");
        }
    });
    mReceiver.setListener( "/move/home", [&] ( const osc::Message &msg ) {
        tiny -> sendGcode("g28.2x0y0z0");
    });
    mReceiver.setListener( "/offset/module", [&] ( const osc::Message &msg ) {
//        tiny -> sendGcode("g28");
//        tiny->setModuleOffest(std::atoi(msg[0].string().c_str()));
        newOffset = std::atoi(msg[0].string().c_str());
        newOffset = true;
    });
    mReceiver.bind();
    mReceiver.listen();
}

int oscControll::getNewOffsetId(){
    newOffset = false;
    return newOffsetId;
}

void oscControll::update(){
    
}