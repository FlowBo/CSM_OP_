//
//  oscControll.cpp
//  CSM_Operator
//
//  Created by Florian Born on 22.01.16.
//
//

#include "oscControll.hpp"

void oscControll::setup( tinyG &t ){
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
        tiny -> setStepSize( msg[0].flt() );
        tiny -> move("g0a");
    });
    mReceiver.setListener( "/control/hold", [&] ( const osc::Message &msg ) {
        tiny->setFeedhold( !tiny->feedhold() );
        if (tiny->feedhold()) {
            tiny->sendGcode("!");
        }else{
            tiny->sendGcode("~");
        }
    });
    mReceiver.bind();
    mReceiver.listen();
}

void oscControll::update(){
    
}