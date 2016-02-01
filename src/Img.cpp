#include "Img.hpp"

void Img::setup(){
    try {
        fs::path path = loadAsset("image.png")->getFilePath();
        if( ! path.empty() ) {
            mImage = gl::Texture::create( loadImage( path ) );
        }
    }
    catch( Exception &exc ) {
        console() << "Could not load file" << endl;
    }
    mSurface = Surface(mImage->createSource());
}

void Img::update(){
    
}
void Img::draw(){
    gl::draw(mImage, Rectf(vec2(0,20),vec2(mImage->getWidth()*3,mImage->getHeight()*3 +20 ) ));
    
}

Color Img::getColorForMirrorId(int id)
{
    int x = id % 30;
    int y = (int)(id / 30);
    ivec2 iv = ivec2( lmap(x, 0, 29, 29, 0), lmap(y, 0, 29, 29, 0));
    ColorA c = mSurface.getPixel(iv);
    return Color(c);
}