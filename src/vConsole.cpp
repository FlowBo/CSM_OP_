//
//  vConsole.cpp
//  CSM_OP_
//
//  Created by Florian Born on 28.01.16.
//
//

#include "vConsole.hpp"

void vConsole::setup(){
    mConsoleFont = Font( "Nitti", 18 );
    mTextureFont = gl::TextureFont::create( mConsoleFont );
}

void vConsole::update(){
    while (consoleStrings.size() > MAX_STRINGS) {
        consoleStrings.erase(consoleStrings.begin());
    }
}

void vConsole::draw(){
    ivec2 v = ivec2(10,getWindowHeight()-10);
    gl::color(ColorA(0.75,0.75,0.75,0.5));
    gl::drawSolidRect(Rectf(vec2( 0, getWindowHeight()-(MAX_STRINGS*LINE_SPACING+30) ),vec2(getWindowWidth()/3,getWindowHeight())));
    gl::color(Color(1.0,1.0,1.0));
    for(vector<string>::reverse_iterator it = consoleStrings.rbegin() ; it != consoleStrings.rend(); ++it) {
        v.y -= LINE_SPACING;
        mTextureFont->drawString(*it,v);
    }
    
}

void vConsole::print(string inputString){
    consoleStrings.push_back(inputString);
    console() << inputString << endl;
}


