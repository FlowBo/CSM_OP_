#include "camera.hpp"

void camera::setup(){
    try {
        mCapture = Capture::create( 1280, 720 );
        mCapture->start();
    }
    catch( ci::Exception &exc ) {
        console() <<  "Failed to init capture " << &exc << endl;
    }
    //init Keystone (need to be replaced by manual Keying)
    mTopLeft        = dvec2(50,50);
    mTopRight       = dvec2(getWindowWidth()-50,50);
    mBottomLeft     = dvec2(50,getWindowHeight()-50);
    mBottomRight    = dvec2(getWindowWidth()-50,getWindowHeight()-50);
    //init Color Vectors
    for (double xP = 0; xP < 30; xP += 1) {
        for (double yP = 0; yP < 30; yP += 1) {
            mMirrorVirtualPosition.push_back( dvec2( lmap( xP, 0.0 , 30.0, 0.0, 1.0 ), lmap( yP, 0.0, 30.0, 0.0, 1.0 ) ) );
            mRealWorldColors.push_back(Color(0,0,0));
        }
    }
}

void camera::update(){
    if( mCapture && mCapture->checkNewFrame() ) {
        mSurface = mCapture -> getSurface();
        if( ! mTexture ) {
            mTexture = gl::Texture::create( *mCapture->getSurface(), gl::Texture::Format().loadTopDown() );
        }
        else {
            mTexture->update( *mCapture->getSurface() );
        }
        calculateColors();
    }
}

void camera::calculateColors(){
    mMirrorPositionInterpolated.clear();
    for(std::vector<vec2>::iterator it = mMirrorVirtualPosition.begin(); it != mMirrorVirtualPosition.end(); ++it){
        mMirrorPositionInterpolated.push_back(interpolateVec(vec2( (*it).x, (*it).y) ));
    }
    int c = 0;
    for(std::vector<vec2>::iterator it = mMirrorPositionInterpolated.begin(); it != mMirrorPositionInterpolated.end(); ++it) {
        vec2 pixelPosition = vec2((*it).x,(*it).y);
        //FIX STEP
        int step = 10;
        Color newColor = findSurroundingColors(pixelPosition,step);
        mRealWorldColors.at(c) = newColor;
        c++;
    }
}
vec2 camera::interpolateVec(dvec2 vec){
    // remapping
    float topX      = lmap(vec.x, 0.0, 1.0, mTopLeft.x, mTopRight.x);
    float bottomX   = lmap(vec.x, 0.0, 1.0, mBottomLeft.x, mBottomRight.x);
    float leftY     = lmap(vec.y, 0.0, 1.0, mTopLeft.y, mBottomLeft.y);
    float rightY    = lmap(vec.y, 0.0, 1.0, mTopRight.y, mBottomRight.y);
    // interpolation
    vec2 interpolatedVector = vec2(lerp(topX, bottomX, vec.y), lerp(leftY, rightY, vec.x) );
    return interpolatedVector;
}

Color camera::findSurroundingColors( vec2 vec, float searchDiameter ){
    double r        = 0;
    double g        = 0;
    double b        = 0;
    int colorCount  = searchDiameter * searchDiameter;
    for (int i = 0; i < searchDiameter; i++) {
        for (int j = 0; j < searchDiameter; j++) {
            ivec2 newPosition = ivec2( abs(vec.x + i), abs(vec.y + i) );
            ColorA c = mSurface->getPixel(newPosition);
            r += c.r;
            g += c.g;
            b += c.b;
        }
    }
    r /= colorCount;
    g /= colorCount;
    b /= colorCount;
    Color mirrorColor = Color(r,g,b);
    return mirrorColor;
}

void camera::draw(){
    if( mTexture ) {
        gl::draw( mTexture );
    }
    drawRealWorldColors();
}

void camera::drawRealWorldColors(){
    int c = 0;
    for(std::vector<vec2>::iterator it = mMirrorPositionInterpolated.begin(); it != mMirrorPositionInterpolated.end(); ++it) {
        ivec2 vec = ivec2( (*it).x, (*it).y );
        //TODO -> FIX SIZE!
        gl::color(mRealWorldColors.at(c));
        gl::drawSolidRect(Rectf(vec,vec + ivec2(15,15)));
        c++;
    }
    gl::color(1,1,1);
}



