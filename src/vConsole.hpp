#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define MAX_STRINGS 25
#define LINE_SPACING 20

class vConsole {
public:
    vConsole(){};
    void setup();
    void update();
    void draw();
    void print(string inputString);
private:
    vector<string> consoleStrings;
    Font mConsoleFont;
    gl::TextureFontRef mTextureFont;
};