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
    for(vector<string>::reverse_iterator it = consoleStrings.rbegin() ; it != consoleStrings.rend(); ++it) {
        v.y -= LINE_SPACING;
        mTextureFont->drawString(*it,v);
    }
}

void vConsole::print(string inputString){
    consoleStrings.push_back(inputString);
}