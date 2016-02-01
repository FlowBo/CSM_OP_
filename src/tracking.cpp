#include "tracking.hpp"

void tracking::setup(){
    
    mWindow = getWindow();
    mCbMouseDown = mWindow->getSignalMouseDown().connect( std::bind( &tracking::mouseDown, this, std::placeholders::_1 ) );
    mCbMouseDrag = mWindow->getSignalMouseDrag().connect( std::bind( &tracking::mouseDrag, this, std::placeholders::_1 ) );
    mCbMouseMove = mWindow->getSignalMouseMove().connect( std::bind( &tracking::mouseMove, this, std::placeholders::_1 ) );
    mCbMouseUp = mWindow->getSignalMouseUp().connect( std::bind( &tracking::mouseUp, this, std::placeholders::_1 ) );
    
    mCbKeyDown = mWindow->getSignalKeyDown().connect( std::bind( &tracking::keyDown, this, std::placeholders::_1 ) );
    mCbKeyUp = mWindow->getSignalKeyUp().connect( std::bind( &tracking::keyUp, this, std::placeholders::_1 ) );
    mCbResize = mWindow->getSignalResize().connect(std::bind(&tracking::resize,this));
    
    mUseBeginEnd = false;
    // initialize warps
    mSettings = getAssetPath( "" ) / "warps.xml";
    if( fs::exists( mSettings ) ) {
        mWarps = Warp::readSettings( loadFile( mSettings ) );
    }
    else {
        mWarps.push_back( WarpPerspective::create() );
    }
    
    mTexturePlaceholder = gl::Texture::create(500,500);

    // load test image
    try {
        mImage = gl::Texture::create( loadImage( loadAsset( "help.png" ) ),
                                     gl::Texture2d::Format().loadTopDown().mipmap( true ).minFilter( GL_LINEAR_MIPMAP_LINEAR ) );
        
        mSrcArea = mImage->getBounds();
        
        // adjust the content size of the warps
        Warp::setSize( mWarps, mImage->getSize() );
    }
    catch( const std::exception &e ) {
        console() << e.what() << std::endl;
    }
}

void tracking::resize()
{
    // tell the warps our window has been resized, so they properly scale up or down
    Warp::handleResize( mWarps );
}

void tracking::cleanup()
{
    // save warp settings
    Warp::writeSettings( mWarps, writeFile( mSettings ) );
}

void tracking::update(){
    
}

void tracking::draw(){

    gl::color( ColorA(0.0,1.0,1.0,0.5));
    if( mImage && Warp::isEditModeEnabled() ) {
        // iterate over the warps and draw their content
        for( auto &warp : mWarps ) {
            // there are two ways you can use the warps:
            warp->draw( mImage, mSrcArea );
        }
    }
    gl::color(ColorA(1.0,1.0,1.0,1.0));

}

void tracking::mouseMove( MouseEvent event )
{
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseMove( mWarps, event ) ) {
        // let your application perform its mouseMove handling here
    }
}

void tracking::mouseDown( MouseEvent event )
{
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseDown( mWarps, event ) ) {
        // let your application perform its mouseDown handling here
    }
}

void tracking::mouseDrag( MouseEvent event )
{
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseDrag( mWarps, event ) ) {
        // let your application perform its mouseDrag handling here
    }
}

void tracking::mouseUp( MouseEvent event )
{
    // pass this mouse event to the warp editor first
    if( !Warp::handleMouseUp( mWarps, event ) ) {
        // let your application perform its mouseUp handling here
    }
}

void tracking::keyDown( KeyEvent event )
{
    // pass this key event to the warp editor first
    if( !Warp::handleKeyDown( mWarps, event ) ) {
        // warp editor did not handle the key, so handle it here
        switch( event.getCode() ) {
            case KeyEvent::KEY_ESCAPE:
                // quit the application
                
                break;
            case KeyEvent::KEY_f:
                // toggle full screen
                mNewKeystone = true;
                setFullScreen( !isFullScreen() );
                break;
            case KeyEvent::KEY_w:
                // toggle warp edit mode
                if (Warp::isEditModeEnabled()) {
                    mNewKeystone = true;
                }
                Warp::enableEditMode( !Warp::isEditModeEnabled() );
                break;
            case KeyEvent::KEY_s:
                cleanup();
                break;
        }
    }
}
void tracking::keyUp( KeyEvent event )
{
    // pass this key event to the warp editor first
    if( !Warp::handleKeyUp( mWarps, event ) ) {
        // let your application perform its keyUp handling here
    }
}

bool tracking::newKeystone(){
    if(mNewKeystone){
        mNewKeystone = false;
        return true;
    }
    return mNewKeystone;
}

vector<dvec2> tracking::getKeystone(){
    vector<dvec2> v;
    for (int i = 0; i < mWarps.at(0)->getNumControlPoints(); i++) {
        dvec2 vec = mWarps.at(0)->getControlPoint(i);
        vec.x *= getWindowWidth();
        vec.y *= getWindowHeight();
        v.push_back(vec);
    }
    return v;
}


