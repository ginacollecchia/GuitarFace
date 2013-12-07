//
//  gf-gfx.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include "gf-gfx.h"
#include "gf-globals.h"
#include "gf-sim.h"

#include "x-fun.h"
#include "x-gfx.h"
#include "x-loadlum.h"
#include "x-vector3d.h"
#include<iostream>

using namespace std;

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc();
void displayFunc();
void reshapeFunc( int width, int height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void specialFunc( int key, int x, int y );

void initialize_graphics();
void initialize_simulation();
bool initialize_data();
void loadTextures();
bool checkTexDim( int dim );
void setupTexture( const char * filename, GLuint texobj,
                  GLenum minFilter, GLenum maxFilter,
                  int useMipMaps = 0 );

void renderBackground();
void blendPane();
void updateNodeEntities();
void renderNodeEntities();


//-----------------------------------------------------------------------------
// name: gf_gfx_init( )
// desc: graphics/interaction entry point
//-----------------------------------------------------------------------------
bool gf_gfx_init( int argc, const char ** argv )
{
#ifdef __APPLE__
    // save working dir
    char * cwd = getcwd( NULL, 0 );
    // set path
    // #ifdef __VQ_RELEASE__
#if 1
    // HACK: use this hard-coded path
    Globals::path = "./";
#else
    // store this globally
    Globals::path = cwd;
#endif
    // compute the datapath
    Globals::datapath = Globals::path + Globals::relpath;
#endif
    
    // initialize GLUT
    glutInit( &argc, (char **)argv );
    
#ifdef __APPLE__
    //restore working dir
    chdir( cwd );
    free( cwd );
#endif
    
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( Globals::windowWidth, Globals::windowHeight );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "GuitarFace");
    // full screen
    if( Globals::fullscreen )
        glutFullScreen();
    
    // set the idle function - called when idle
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    // for arrow keys, etc
	glutSpecialFunc (specialFunc );
    
    // do our own initialization
    initialize_graphics();
    // do data
    if( !initialize_data() )
    {
        // done
        return false;
    }
    // simulation
    initialize_simulation();

    
    
    return true;
}




//-----------------------------------------------------------------------------
// name: gf_gfx_loop( )
// desc: hand off to graphics loop
//-----------------------------------------------------------------------------
void gf_gfx_loop()
{
    // let GLUT handle the current thread from here
    glutMainLoop();
}




//-----------------------------------------------------------------------------
// Name: initialize_graphics( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize_graphics()
{
    // log
    cerr << "Initializing graphics system..." << endl;
    
    // reset time
    XGfx::resetCurrentTime();
    // set simulation speed
    XGfx::setDeltaFactor( 1.0f );
    // get the first
    XGfx::getCurrentTime( true );
    // random
    XFun::srand();
    
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // set fill mode
    glPolygonMode( GL_FRONT_AND_BACK, Globals::fillmode );
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable lighting for front
    glLightModeli( GL_FRONT_AND_BACK, GL_TRUE );
    // material have diffuse and ambient lighting
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );
    // normalize (for scaling)
    glEnable( GL_NORMALIZE );
    // line width
    glLineWidth( Globals::linewidth );
    
    // enable light 0
    glEnable( GL_LIGHT0 );
    
    // setup and enable light 1
    glLightfv( GL_LIGHT1, GL_AMBIENT, Globals::light1_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, Globals::light1_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, Globals::light1_specular );
    glEnable( GL_LIGHT1 );
    
    // load textures
    loadTextures();
    
    // fog
    Globals::fog_mode[0] = 0;
    Globals::fog_mode[1] = GL_LINEAR;
    // fog_mode[1] = GL_EXP; fog_mode[2] = GL_EXP2;
    Globals::fog_filter = 0;
    Globals::fog_density = .04f;
    
    // fog color
    GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};
    // fog mode
    if( Globals::fog_filter ) glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
    // set fog color
    glFogfv( GL_FOG_COLOR, fogColor );
    // fog density
    glFogf( GL_FOG_DENSITY, Globals::fog_density );
    // fog hint
    glHint( GL_FOG_HINT, GL_DONT_CARE );
    // fog start depth
    glFogf( GL_FOG_START, 1.0f );
    // fog end depth
    glFogf( GL_FOG_END, 10.5f );
    // enable
    if( Globals::fog_filter ) glEnable( GL_FOG );
    
    // check global flag
    if( Globals::fog )
    {
        // fog mode
        glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
        // enable
        glEnable(GL_FOG);
    }
    else
    {
        // disable
        glDisable(GL_FOG);
    }
    
    // clear the color buffer once
    glClear( GL_COLOR_BUFFER_BIT );
}




//-----------------------------------------------------------------------------
// name: initialize_simulation( )
// desc: simulation setup
//-----------------------------------------------------------------------------
void initialize_simulation()
{
    
    Globals::sim = new GFSim();
    GFCameraWall *camwall = new GFCameraWall();
    camwall->initCamera();
    
    GFBackgroundImage *bimage = new GFBackgroundImage("tunnel_cropped.png");
    
    GFInfoBar *pbar = new GFInfoBar();
    GFTunnel *tunnel = new GFTunnel();
        
    Globals::sim->root().addChild( pbar );
    Globals::sim->root().addChild( tunnel );
    Globals::sim->root().addChild( camwall );
    Globals::sim->root().addChild( bimage );
    
}




//-----------------------------------------------------------------------------
// name: initialize_data( )
// desc: load initial data
//-----------------------------------------------------------------------------
bool initialize_data()
{
    Globals::data = new GFNoteStore();    
    return true;
}


//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( int w, int h )
{
    // save the new window size
    Globals::windowWidth = w; Globals::windowHeight = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
}




//-----------------------------------------------------------------------------
// Name: look( )
// Desc: ...
//-----------------------------------------------------------------------------
void look( )
{
    // go
//    Globals::fov.interp( XGfx::delta() );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( Globals::fov.value, (GLfloat)Globals::windowWidth / (GLfloat)Globals::windowHeight, .005, 500.0 );
    
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity();
    // position the view point
//    gluLookAt( 0.0f,
//              Globals::viewRadius.x * sin( Globals::viewEyeY.x ),
//              Globals::viewRadius.x * cos( Globals::viewEyeY.x ),
//              0.0f, 0.0f, 0.0f,
//              0.0f, ( cos( Globals::viewEyeY.x ) < 0 ? -1.0f : 1.0f ), 0.0f );

    gluLookAt( 0.0f,
              0.0f ,
              3.0f ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f , 0.0f );

    // set the position of the lights
    glLightfv( GL_LIGHT0, GL_POSITION, Globals::light0_pos );
    glLightfv( GL_LIGHT1, GL_POSITION, Globals::light1_pos );
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    // system keys (handled first)
    switch( key )
    {
        case 'q':
        {
            exit( 0 );
            break;
        }
        case 'd':
        {
            GFOverlayMessage *msg = new GFOverlayMessage("fuck_yeah.png");
            Globals::sim->root().addChild( msg);
            msg->loc.z = -4;
            break;
        }
        case 'f':
        {
            GFOverlayMessage *msg = new GFOverlayMessage("widdly_wahh.png");
            Globals::sim->root().addChild( msg);
            msg->loc.z = -4;
            break;
        }
        case 'g':
        {
            GFOverlayMessage *msg = new GFOverlayMessage("nice.png");
            Globals::sim->root().addChild( msg);
            msg->loc.z = -4;
            break;
        }
        case 'h':
        {
            GFOverlayMessage *msg = new GFOverlayMessage("1003_points.png");
            Globals::sim->root().addChild( msg);
            msg->loc.z = -4;
            break;
        }

    }
    // do a reshape since viewEyeY might have changed
    reshapeFunc( Globals::windowWidth, Globals::windowHeight );
    // post redisplay
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: specialFunc( )
// Desc: handles arrow stuff
//-----------------------------------------------------------------------------
void specialFunc( int key, int x, int y )
{
    // check
    bool handled = false;
    
    // if not handled
    if( !handled )
    {
        switch( key )
        {
            case GLUT_KEY_LEFT:
                break;
            case GLUT_KEY_RIGHT:
                break;
            case GLUT_KEY_UP:
                break;
            case GLUT_KEY_DOWN:
                break;
        }
    }
}




//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    // update time
    XGfx::getCurrentTime( TRUE );
    
    // update
    Globals::bgColor.interp( XGfx::delta() );
    Globals::blendAlpha.interp( XGfx::delta() );
    
    // clear or blend
    if( Globals::blendScreen && Globals::blendAlpha.value > .0001 )
    {
        // clear the depth buffer
        glClear( GL_DEPTH_BUFFER_BIT );
        // blend screen
        blendPane();
    }
    else
    {
        // set the GL clear color - use when the color buffer is cleared
        glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
        // clear the color and depth buffers
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }
    
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    
    // save state
    glPushMatrix();
    
    // slew
    Globals::viewEyeY.interp( XGfx::delta());
    Globals::viewRadius.interp( XGfx::delta() );
    look();
    
    // cascade simulation
    Globals::sim->systemCascade();
    
    // pop state
    glPopMatrix();
    
    //    // draw any HUD here
    //    Globals::hud->project();
    //    Globals::hud->updateAll( Globals::sim->delta() );
    //    Globals::hud->drawAll();
    
    // flush gl commands
    glFlush();
    // swap the buffers
    glutSwapBuffers();
}




//-----------------------------------------------------------------------------
// name: blendPane()
// desc: blends a pane into the current scene
//-----------------------------------------------------------------------------
void blendPane()
{
    // enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // disable lighting
    glDisable( GL_LIGHTING );
    // disable depth test
    glDisable( GL_DEPTH_TEST );
    // blend in a polygon
    glColor4f( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, Globals::blendAlpha.value );
    // glColor4f( Globals::blendRed, Globals::blendRed, Globals::blendRed, Globals::blendAlpha );
    // reduce the red component
    // Globals::blendRed -= .02f;
    // if( Globals::blendRed < 0.0f ) Globals::blendRed = 0.0f;
    
    GLfloat h = 10;
    GLfloat d = -1;
    
    // draw the polyg
    glBegin( GL_QUADS );
    glVertex3f( -h, -h, d );
    glVertex3f( h, -h, d );
    glVertex3f( h, h, d );
    glVertex3f( -h, h, d );
    glEnd();
    
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    // disable blending
    glDisable( GL_BLEND );
}




//-----------------------------------------------------------------------------
// name: renderBackground()
// desc: ...
//-----------------------------------------------------------------------------
void renderBackground()
{
    // save the current matrix
    glPushMatrix( );
    
    // restore
    glPopMatrix( );
}




//-------------------------------------------------------------------------------
// name: loadTexture()
// desc: load textures
//-------------------------------------------------------------------------------
void loadTextures()
{
    char filename[256];
    GLenum minFilter, maxFilter;
    int i;
    
    // log
    fprintf( stderr, "[bokeh]: loading textures...\n" );
    
    // set store alignment
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    // set filter types
    minFilter = GL_LINEAR;
    maxFilter = GL_LINEAR;
    
//    // load tng flares
//    for( i = RAKA_TEX_FLARE_TNG_1; i <= RAKA_TEX_FLARE_TNG_2; i++ )
//    {
//        glGenTextures( 1, &(Globals::textures[i]) );
//        sprintf( filename, "%smsg-%d.bmp", Globals::datapath.c_str(), i - RAKA_TEX_FLARE_TNG_1 + 1 );
//        setupTexture( filename, Globals::textures[i], minFilter, maxFilter );
//    }
}




//-------------------------------------------------------------------------------
// name: gf_loadTexture()
// desc: load texture
//-------------------------------------------------------------------------------
XTexture * gf_loadTexture( const string & filename )
{
    // instantiate image data
    XTexture * tex = new XTexture;
    
    // load the texture
    if( !gf_initTexture( filename, tex ) )
    {
        delete tex;
        return NULL;
    }
    
    return tex;
}




//-------------------------------------------------------------------------------
// name: gf_loadTexture()
// desc: load texture
//-------------------------------------------------------------------------------
bool gf_initTexture( const string & filename, XTexture * tex )
{
    // set desired resize
    tex->resizeWidth = 512;
    tex->resizeHeight = 512;
    
    // generate the texture
    glGenTextures( 1, &tex->name );
    // bind the texture
    glBindTexture( GL_TEXTURE_2D, tex->name );
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // setting parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // load the texture
    if( !XGfx::loadTexture( filename, tex ) )
    {
        cerr << "[bokeh]: error - cannot load texture '" << filename.c_str() << "'..." << endl;
        return false;
    }
    
    return true;
}




//--------------------------------------------------------------------------------
// name: setupTexture()
// desc: ...
//--------------------------------------------------------------------------------
void setupTexture( const char * filename, GLuint texobj,
                  GLenum minFilter, GLenum maxFilter, int useMipMaps )
{
    unsigned char * buf = NULL;
    int width = 0, height = 0, components = 0;
    
    glBindTexture( GL_TEXTURE_2D, texobj );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter );
    
    // log
    // fprintf( stderr, "[bokeh]: loading %s...\n", filename );
    
    // load luminance
    buf = loadLuminance( filename, &width, &height, &components );
    
    // log
    // fprintf( stderr, "[bokeh]: '%s' : %dx%dx%d\n", filename, width, height, components);
    
    // build mip maps
    if( useMipMaps )
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, 1, width, height,
                          GL_LUMINANCE, GL_UNSIGNED_BYTE, buf );
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, 1, width, height, 0,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, buf );
    }
    
    free(buf);
}




//-----------------------------------------------------------------------------
// name: checkTexDim( )
// desc: checks to see if a dim is a valid opengl texture dimension
//-----------------------------------------------------------------------------
bool checkTexDim( int dim )
{
    if( dim < 0 )
        return false;
    
    int i, count = 0;
    
    // count bits
    for( i = 0; i < 31; i++ )
        if( dim & ( 0x1 << i ) )
            count++;
    
    // this is true only if dim is power of 2
    return count == 1;
}
