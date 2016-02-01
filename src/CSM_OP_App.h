#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/cairo/Cairo.h"
#include "oscControll.hpp"
#include "visualController.hpp"
#include "vConsole.hpp"
#include "GUI.h"
#include <iostream>
#include "tinyG.hpp"
#include "MainController.hpp"

#define ENABLE_TINYG 1
#define CAMERA 1

using namespace ci;
using namespace ci::app;
using namespace std;

vConsole vconsole;
int value;

class CSM_OP_App : public App {
public:
    void setup() override;
    void mouseDown( MouseEvent event ) override;
    void update() override;
    void draw() override;
    void keyDown(KeyEvent event) override;
    
    MainController      controller;
    tinyG               tiny;
    oscControll         osc;
    visualController    vc;
    
    float   mFramerate;
    Font    mFont;
    int     roundTest;
};
