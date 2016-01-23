

#include "camera.hpp"


void camera::setup(){
    try {
        
        mCapture = Capture::create( 1280, 720 );
        mCapture->start();
    }
    catch( ci::Exception &exc ) {
        console() <<  "Failed to init capture " << &exc << endl;
    }
}

void camera::update(){
    if( mCapture && mCapture->checkNewFrame() ) {
        if( ! mTexture ) {
            // Capture images come back as top-down, and it's more efficient to keep them that way
            mTexture = gl::Texture::create( *mCapture->getSurface(), gl::Texture::Format().loadTopDown() );
        }
        else {
            mTexture->update( *mCapture->getSurface() );
        }
    }
}

void camera::draw(){
    if( mTexture ) {
//        gl::ScopedModelMatrix modelScope;
        gl::draw( mTexture );
    }
}