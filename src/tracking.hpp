#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "Warp.h"

using namespace ci;
using namespace ci::app;
using namespace ph::warping;
using namespace std;

class tracking{
public:
    tracking(){};
    void setup();
    void update();
    void draw();
    void cleanup();
    void resize();
    
    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseUp( MouseEvent event );
    
    void keyDown( KeyEvent event );
    void keyUp( KeyEvent event );
    bool newKeystone();
    vector<dvec2> getKeystone();
    
private:
    bool			mUseBeginEnd;
    bool            mNewKeystone;
    fs::path		mSettings;
    gl::TextureRef	mImage;
    gl::TextureRef  mTexturePlaceholder;
    Surface8uRef    mSurfacePlaceholder;
    WarpList		mWarps;
    Area			mSrcArea;
    ci::app::WindowRef           mWindow;
    ci::signals::ScopedConnection mCbMouseDown, mCbMouseDrag, mCbMouseMove, mCbMouseUp;
    ci::signals::ScopedConnection mCbKeyDown, mCbKeyUp;
    ci::signals::ScopedConnection mCbResize;
    
};