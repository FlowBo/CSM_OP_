#include "camera.hpp"


class visualController{
public:
    visualController(){};
    void setup();
    void update();
    void draw();
    
private:
    camera cam;
};