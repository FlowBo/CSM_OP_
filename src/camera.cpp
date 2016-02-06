#include "camera.hpp"

void camera::setup(){
    try {
        mCapture = Capture::create( 1280, 720 );
        mCapture->start();
        mUseCamera = true;
    }
    catch( ci::Exception &exc ) {
        console() <<  "Failed to init capture " << &exc << endl;
        mUseCamera = false;
    }
    
    //init Color Vectors
    for (double yP = 0; yP < 30; yP += 1) {
        for (double xP = 0; xP < 30; xP += 1) {
            mMirrorVirtualPosition.push_back( dvec2( lmap( xP, 0.0 , 30.0, 1.0, 0.0 ), lmap( yP, 0.0, 30.0, 1.0, 0.0 ) ) );
            mRealWorldColors.push_back(Color(0,0,0));
        }
    }
}

void camera::setKeystone(vector<dvec2> corners)
{
    mTopLeft = corners.at(0);
    mTopRight = corners.at(1);
    mBottomRight = corners.at(2);
    mBottomLeft = corners.at(3);
    
    mInstallationWidth = ((mTopRight.x+mBottomRight.x)/2) - ((mTopLeft.x+mBottomLeft.x)/2)  ;
//    mMirrorSize = (mInstallationWidth/30)*0.76923 ;
    mMirrorSize = (mInstallationWidth/30)*0.7 ;
    
    
    
    mTLsurface = dvec2(
                       lmap(mTopLeft.x, 0.0, (double)getWindowWidth(), 0.0, (double)mCapture->getWidth() ),
                       lmap(mTopLeft.y, 0.0, (double)getWindowHeight(), 0.0, (double)mCapture->getHeight() ));
    mTRsurface = dvec2(
                       lmap(mTopRight.x, 0.0, (double)getWindowWidth(), 0.0, (double)mCapture->getWidth() ),
                       lmap(mTopRight.y, 0.0, (double)getWindowHeight(), 0.0, (double)mCapture->getHeight() ));
    mBLsurface = dvec2(
                       lmap(mBottomLeft.x, 0.0, (double)getWindowWidth(), 0.0, (double)mCapture->getWidth() ),
                       lmap(mBottomLeft.y, 0.0, (double)getWindowHeight(), 0.0, (double)mCapture->getHeight() ));
    mBRsurface = dvec2(
                       lmap(mBottomRight.x, 0.0, (double)getWindowWidth(), 0.0, (double)mCapture->getWidth() ),
                       lmap(mBottomRight.y, 0.0, (double)getWindowHeight(), 0.0, (double)mCapture->getHeight() ));
    calculateMirrorPositions();

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

void camera::calculateMirrorPositions(){
    mMirrorPositionInterpolated.clear();
    for(std::vector<vec2>::iterator it = mMirrorVirtualPosition.begin(); it != mMirrorVirtualPosition.end(); ++it){
        mMirrorPositionInterpolated.push_back(interpolateVec(vec2( (*it).x, (*it).y) ));
    }
    createMirrorScreenPosition();
}

void camera::createMirrorScreenPosition(){
    mMirrorPositionScreenPosition.clear();
    for(auto mirrorPos : mMirrorPositionInterpolated)
    {
        dvec2 v = dvec2(
                        lmap((double)mirrorPos.x, 0.0, (double)mCapture->getWidth(), 0.0, (double)getWindowWidth()),
                        lmap((double)mirrorPos.y, 0.0, (double)mCapture->getHeight(), 0.0, (double)getWindowHeight()));
        mMirrorPositionScreenPosition.push_back(v);
    }
}

void camera::calculateColors(){
    
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
    float topX      = lmap(vec.x, 0.0, 1.0, mTLsurface.x, mTRsurface.x);
    float bottomX   = lmap(vec.x, 0.0, 1.0, mBLsurface.x, mBRsurface.x);
    float leftY     = lmap(vec.y, 0.0, 1.0, mTLsurface.y, mBLsurface.y);
    float rightY    = lmap(vec.y, 0.0, 1.0, mTRsurface.y, mBRsurface.y);
    
    
    // interpolation
    vec2 interpolatedVector = vec2(lerp(topX, bottomX, vec.y), lerp(leftY, rightY, vec.x) );
    vec2 v = vec2(interpolatedVector.x ,interpolatedVector.y);
    return v;
}

Color camera::findSurroundingColors( vec2 vec, float searchDiameter ){
    double r        = 0;
    double g        = 0;
    double b        = 0;
    int colorCount  = searchDiameter * searchDiameter;
    int halfSearchDiameter = (int)searchDiameter/2;
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
//      f  gl::draw( mTexture );
        gl::draw( mTexture, Rectf(vec2(0.0,0.0), getWindowSize()));
    }
    drawRealWorldColors();
}

void camera::drawRealWorldColors(){
    int c = 0;
    for(std::vector<dvec2>::iterator it = mMirrorPositionScreenPosition.begin(); it != mMirrorPositionScreenPosition.end(); ++it) {
        ivec2 vec = ivec2( (*it).x, (*it).y);
        ivec2 vec2 = ivec2( (*it).x + (int)mMirrorSize/2, (*it).y + + (int)mMirrorSize/2 );
        //TODO -> FIX SIZE!
        gl::color(mRealWorldColors.at(c));
        gl::drawSolidRect(Rectf(vec,vec + ivec2(mMirrorSize,mMirrorSize)));
        gl::color(ColorA(0.0,1.0,1.0,0.7));
//        gl::drawSolidCircle(vec, 1.5);
//        stringstream ss;
//        ss << c;
//        gl::drawString( ss.str(), vec);
        c++;
    }
    gl::color(1,1,1);
}


Color camera::getColorOfMirror(int id){
    return mRealWorldColors.at(id);
}
