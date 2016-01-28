#include "camera.hpp"


class visualController{
public:
    visualController(){};
    void setup();
    void update();
    void draw();
    Surface8uRef getCameraImage(){return cam.getCameraImage();};
    
private:
    camera cam;
};