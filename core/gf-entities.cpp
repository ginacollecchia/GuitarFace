//
//  gf-entities.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//
#include <iostream>
#include "gf-entities.h"
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

// bokehs
vector<YBokeh *> g_tunnels;

// max notes per hour (pace meter just displays as full)
double maxNPH = 10000.0f;

//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void GFTeapot::update( YTimeInterval dt )
{
    // do nothing for now
}




//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void GFTeapot::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glutSolidTeapot( 1.0 );
    // disable lighting
    glDisable( GL_LIGHTING );
}

GFInfoBar::GFInfoBar(){
    a = new YText(1.0);
    this->addChild(a);
    b = new YText(1.0);
    this->addChild(b);
    c = new YText(1.0);
    this->addChild(c);
    d = new YText(1.0);
    this->addChild(d);
    e = new YText(1.0);
    this->addChild(e);
}

void GFInfoBar::render(){
    // enable lighting
    
    glEnable( GL_LIGHTING );
    // set color
    
    ostringstream s1;
    s1<<"Notes: "<<Globals::data->m_note_count; // << "/"<<Globals::note_goal;
    a->set(s1.str());
    a->loc.x = -3.4;
    a->loc.y = -1.8;
    
    /* ostringstream s2;
    s2<<"NPH: " << Globals::data->getNotesPerHour();
    b->set(s2.str());
    b->loc.x = -2.55;
    b->loc.y = -1.8; */

    ostringstream s3;
    s3<<"Power Chords: "<<Globals::data->m_power_chord_count;
    c->set(s3.str());
    c->loc.x = -2.4;
    c->loc.y = -1.8;

    ostringstream s4;
    s4<<"Jumps: " << Globals::data->m_jump_count;
    d->set(s4.str());
    d->loc.x = -0.85;
    d->loc.y = -1.8;
    
    ostringstream s5;
    s5<<"Pace: ";
    e->set(s5.str());
    e->loc.x = 0.0;
    e->loc.y = -1.8;
    
    // s6<<"Pitch Classes: ";
    // f->set(s6.str());
    // f->loc.x = 0.85;
    // f->loc.y = -1.8;
    
    // s7<<"Div. of Beat: ";
    // g->set(s7.str());
    // g->loc.x = 1.7;
    // g->loc.y = -1.8;

    
    // border around info bar
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glBegin(GL_LINE_LOOP);
    glVertex2f(-3.5, -2);
    glVertex2f(-3.5, -1.5);
    glVertex2f(3.5, -1.5);
    glVertex2f(3.5, -2);
    glEnd();
    
    int n;
    // dynamic range is calculated from local velocities: need algorithm here (circular buffer)
    ( Globals::idx >= 10 ) ? n = 10 : n = Globals::idx;
    
    // paceX is calculated over the last 10 velocities
    for(int i = 0; i < n; i++ )
    {
        paceX += sqrt(Globals::data->m_time_stamps[Globals::idx - i]) / n;
    }

    if ( paceX > 0.5f )
        paceX = 0.5f;
    // cout << "Pace: " << paceX << endl;
    
    // pace meter
    glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(0.4, -1.7);
    glVertex2f(0.4, -1.9);
    glVertex2f(paceX + 0.4, -1.9);
    glVertex2f(paceX + 0.4, -1.7);
    glEnd();
    
    glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(paceX + 0.4, -1.7);
    glVertex2f(paceX + 0.4, -1.9);
    glVertex2f(0.9, -1.9);
    glVertex2f(0.9, -1.7);
    glEnd();
    
    // border of pace meter
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.39, -1.69);
    glVertex2f(0.39, -1.91);
    glVertex2f(0.91, -1.91);
    glVertex2f(0.91, -1.69);
    glEnd();
    
    // disable lighting
    glDisable( GL_LIGHTING );
}

void GFInfoBar::update( YTimeInterval dt )
{
        // do nothing for now
}

GFTunnelLayer::GFTunnelLayer(GLfloat _size, Vector3D _color, GLfloat _deltaz):size(_size), color(_color), deltaz(_deltaz){
    
}
GFTunnelLayer::~GFTunnelLayer(){
    
}

void GFTunnelLayer::update( YTimeInterval dt ){
    
}

void GFTunnelLayer::render(){
    
//    bokeh = new YBokeh();
//    // set attributes
//    bokeh->set( size, 1.0f, 1.0f, 1.0f, TUNNEL_BOKEH_1 );
//    bokeh->sca.set( 1, 1, 1 );
//    // set bokeh
//    bokeh->setBokehParams( // initial time
//                          2,
//                          // freq
//                          0,
//                          // time step
//                          50,
//                          // location
//                          Vector3D( 0, 0, 0 ), // Vector3D(XFun::rand2f(-1,1),XFun::rand2f(-1,1), XFun::rand2f(-1,1)),
//                          // color
//                          color );
//    // alpha
//    bokeh->setAlpha( 1 );
//    // add to simulation
//    
    this->loc.z -= deltaz;
    circle(0, 0, 2, 12);
    if(this->loc.z < -10){
        this->active = false;
    }
    
//    this->addChild( bokeh );
    
}

GFTexture::GFTexture(string _filename):filename(_filename){
    string path = "../../data/texture/";
    texture = gf_loadTexture(path + filename);
}

GFTexture::~GFTexture(){
    
}
void GFTexture::update(YTimeInterval dt){
    
    
}

void GFTexture::render(){
    
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture->name); //bind the texture
    glBegin (GL_QUADS);
    
    double ratio = texture->origWidth/texture->origHeight;
    double scale = 0.5;
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,0.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,0.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,0.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,0.0);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    /* attempts to start black, pause, then fade to black. doesn't work (image never displays).
     this->alpha = 0.0f;
     applyTransforms();
     
     if ( Globals::messageMove && this->alpha < 0.96 )
     {
     this->loc.z += 0.01;
     this->alpha += 0.05;
     applyTransforms();
     }
     
     if (this->alpha >= 0.96 )
     {
     sleep(1);
     this->alpha -= 0.1;
     applyTransforms();
     }
     
     if(this->loc.z > 0){
     // sleep(2);
     this->active = false;
     } */
    
    if ( this->loc.z > 0 ) {
        this->active = false;
    }
}

void GFNoteObject::render(){
    // enable lighting
    glEnable(GL_LIGHTING);
    
    // pick one
    // glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Globals::light1_diffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    // glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    // position the lighting
    glLightfv(GL_LIGHT1, GL_POSITION, Globals::light0_pos);
    
    // attenuate the lighting
    // glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
    // glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    
    this->loc.z -= 0.01;
    float deg = 2*M_PI*(float)pitch/12.0f;
    glTranslatef(2 * sin(deg), 2*cos(deg),1);

    if (Globals::data->m_power_chord)
    {
        GFTexture *power = new GFTexture("lightning.bw");
        this->addChild( power );
        Globals::data->m_power_chord = false;
    }
    
    
    glColor3f(1,0,0);
    this->alpha -= 0.002;
    glutSolidCube(0.1);
    
    
    glDisable(GL_LIGHTING);
}

void GFNoteObject::update(YTimeInterval dt){
    
}

YTimeInterval GFTunnel::deltatime = 0;

void GFTunnel::update(YTimeInterval dt){
    
    deltatime += dt;
    if (deltatime>0.2){
        GFTunnelLayer *g = new GFTunnelLayer(5.0,Vector3D(0,0.5,0),0.15);
        //        GFTunnelLayer *g2 = new GFTunnelLayer(8.0, Vector3D(0,0,0.5),0.10);
        g->loc.z = 2;
        //        g2->loc.z = 2;
        this->addChild( g );
        //        this->addChild( g2 );
        deltatime = 0;
    }
}

void GFTunnel::render(){
    
    
    while(!Globals::note_queue.empty()){
        GFMIDIEvent e = Globals::note_queue.front();
        Globals::note_queue.pop();
        GFNoteObject *o = new GFNoteObject();
        o->pitch = e.getPitchClass();
        this->addChild(o);
    }
    
    
}

GFCameraWall::GFCameraWall(){

    if (!(camCapture = cvCaptureFromCAM(CV_CAP_ANY))) {
        std::cout << "Failed to capture from camera" << std::endl;
    }
    
    glGenTextures(1, &texture);
    
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720,0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    
    std::cout << "Camera opened successfully" << std::endl;
    
}

GFCameraWall::~GFCameraWall(){
    
}


// Simple camera feed. No detection happens here. Look at gf_init_cam_thread in gf-face.h for face rec.

void GFCameraWall::render(){
    
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
    
    double ratio = 1280.0/720.0;
    double scale = 1.5;
    // camera wall is the farthest back thing, at -10.0
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,-3.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,-3.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,-3.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,-3.0);
    glEnd();
    glDisable( GL_TEXTURE_2D );

    
    IplImage *cameraFrame;
    
    if ((cameraFrame = cvQueryFrame(camCapture))) {
        cv::Mat frame, frameCopy;
        frame = cameraFrame;
    
        if( cameraFrame->origin == IPL_ORIGIN_TL ){ //Dont know why this is here.
            cv::flip( frame, frameCopy, -1 ); //This is what we need
        }
        else{
            cv::flip( frame, frameCopy, 0 );
        }
        loadTexture_Mat(&frameCopy, &texture);
    }
}

void GFCameraWall::update(YTimeInterval dt){
    
}

GFVideoPlayer::GFVideoPlayer(string _file){
    file = _file;
    cam.open(file);
    
    if(!cam.isOpened()){
        std::cerr << "Failed opening video file." << endl;
    }
    
    glGenTextures(1, &texture);
    
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // if it's small
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // if it's big
    
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480,0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    
    
    std::cerr << "Camera opened successfully" << std::endl;
}

GFVideoPlayer::~GFVideoPlayer(){
    
}

void GFVideoPlayer::render(){
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
   
    double ratio = 640/480;
    double scale = 2.0;
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,0.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,0.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,0.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    Mat im;
    cam >> im;
    
    loadTexture_Mat(&im, &texture);
}

void GFVideoPlayer::update(YTimeInterval dt){
    
}


//------------------------------------------------------------------------
// name: GFOverlayMessage(filename) constructor
// desc: creates an image object. 
//------------------------------------------------------------------------

GFOverlayMessage::GFOverlayMessage(string _filename):filename(_filename){
    string path = "../../data/texture/";
    texture = gf_loadTexture(path + filename);
}

GFOverlayMessage::~GFOverlayMessage(){
    
}
void GFOverlayMessage::update(YTimeInterval dt){

    
}

void GFOverlayMessage::render(){
    
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture->name); //bind the texture
    glBegin (GL_QUADS);
    
    double ratio = texture->origWidth/texture->origHeight;
    double scale = 2.0;
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,0.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,0.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,0.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,0.0);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    /* attempts to start black, pause, then fade to black. doesn't work (image never displays).
     this->alpha = 0.0f;
    applyTransforms();

    if ( Globals::messageMove && this->alpha < 0.96 )
    {
        this->loc.z += 0.01;
        this->alpha += 0.05;
        applyTransforms();
    }
    
    if (this->alpha >= 0.96 )
    {
        sleep(1);
        this->alpha -= 0.1;
        applyTransforms();
    }
    
    if(this->loc.z > 0){
        // sleep(2);
        this->active = false;
    } */
    
    if ( Globals::messageMove )
    {
        this->loc.z += 0.05;
    }
    
    if ( this->loc.z > 0 ) {
        // hold it here for 3 seconds, then remove; show the intro text towards the end of that
        if ( Globals::showIntroText )
            sleep(3);
        // Globals::showIntroText = true;
        // sleep(1.5);
        this->active = false;
    }
}

//------------------------------------------------------------------------
// name: GFBackgroundImage(filename) constructor
// desc: puts an image texture in the background. unused.
//------------------------------------------------------------------------

GFBackgroundImage::GFBackgroundImage(string _filename):filename(_filename){
    string path = "../../data/texture/";
    texture = gf_loadTexture(path + filename);
}

void GFBackgroundImage::update(YTimeInterval dt){
    
}

void GFBackgroundImage::render(){
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture->name); //bind the texture
    glBegin (GL_QUADS);    
    
    double ratio = texture->origWidth/texture->origHeight;
    double scale = 2.0;
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,0.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,0.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,0.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,0.0);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

GFGuitarFace::GFGuitarFace(Mat _image):image(_image){
}

GFGuitarFace::~GFGuitarFace(){
    
}

// render guitar face mask (Mat image) onto sphere

void GFGuitarFace::render(){
    
    loadTexture_Mat(&image, &texture);
    
    glEnable( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture( GL_TEXTURE_2D, texture );
    glBegin( GL_TRIANGLES );
    
    long PI = 3.14159265359;
    // first vertex
    float tx1, ty1;
    // a is the X, Y, and Z coordinates of the normal
    GLdouble a[] = {1, 0, 0};
    tx1 = atan2(a[0], a[2]) / (2. * PI) + 0.5;
    ty1 = asin(a[1]) / PI + .5;
    
    glTexCoord2f(tx1, ty1); glVertex2d(tx1, ty1);
    
    // second vertex
    float tx2, ty2;
    GLdouble b[] = {0, 0, -1};
    tx2 = atan2(b[0], b[2]) / (2. * PI) + 0.5;
    ty2 = asin(b[1]) / PI + .5;
    if(tx2 < 0.75 && tx1 > 0.75)
        tx2 += 1.0;
    else if(tx2 > 0.75 && tx1 < 0.75)
        tx2 -= 1.0;
    
    glTexCoord2f(tx2, ty2); glVertex2d(tx2, ty2);
    
    // third vertex
    float tx3, ty3;
    GLdouble c[] = {-1, 0, 0};
    tx3 = atan2(c[0], c[2]) / (2. * PI) + 0.5;
    ty3 = asin(c[1]) / PI + .5;
    
    glTexCoord2f(tx3, ty3); glVertex2d(tx3, ty3);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
}


void GFGuitarFace::Sphere(double p_radius)
{
    GLdouble a[] = {1, 0, 0};
    GLdouble b[] = {0, 0, -1};
    GLdouble c[] = {-1, 0, 0};
    GLdouble d[] = {0, 0, 1};
    GLdouble e[] = {0, 1, 0};
    GLdouble f[] = {0, -1, 0};
    
    int recurse = 5;
    
    SphereFace(recurse, p_radius, d, a, e);
    SphereFace(recurse, p_radius, a, b, e);
    SphereFace(recurse, p_radius, b, c, e);
    SphereFace(recurse, p_radius, c, d, e);
    SphereFace(recurse, p_radius, a, d, f);
    SphereFace(recurse, p_radius, b, a, f);
    SphereFace(recurse, p_radius, c, b, f);
    SphereFace(recurse, p_radius, d, c, f);
}


inline void Normalize(GLdouble *v)
{
    GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

//----------------------------------------------------------------------
// Name :         CChildView::SphereFace()
// Description :  Draw a single facet of the sphere.  If p_recurse > 1,
//                triangulate that facet and recurse.
//----------------------------------------------------------------------

void GFGuitarFace::SphereFace(int p_recurse, double p_radius, GLdouble *a, GLdouble *b, GLdouble *c)
{
    if(p_recurse > 1)
    {
        // Compute vectors halfway between the passed vectors
        GLdouble d[3] = {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
        GLdouble e[3] = {b[0] + c[0], b[1] + c[1], b[2] + c[2]};
        GLdouble f[3] = {c[0] + a[0], c[1] + a[1], c[2] + a[2]};
        
        Normalize(d);
        Normalize(e);
        Normalize(f);
        
        SphereFace(p_recurse-1, p_radius, a, d, f);
        SphereFace(p_recurse-1, p_radius, d, b, e);
        SphereFace(p_recurse-1, p_radius, f, e, c);
        SphereFace(p_recurse-1, p_radius, f, d, e);
    }
    
    /* glBegin(GL_TRIANGLES);
    glNormal3dv(a);
    glVertex3d(a[0] * p_radius, a[1] * p_radius, a[2] * p_radius);
    
    glNormal3dv(b);
    glVertex3d(b[0] * p_radius, b[1] * p_radius, b[2] * p_radius);
    
    glNormal3dv(c);
    glVertex3d(c[0] * p_radius, c[1] * p_radius, c[2] * p_radius);
    glEnd(); */
}


void GFGuitarFace::update(YTimeInterval dt){    
}


//------------------------------------------------------------------------
// name: GFCameraWiggle constructor
// desc: Simple camera feed. No detection happens here (see gf-face.cpp).
//------------------------------------------------------------------------

GFCameraWiggle::GFCameraWiggle(){
    GFTrackPlayer *player = new GFTrackPlayer("../../data/widdly.mp3");
    player->play();
}

void GFCameraWiggle::render(){
    frameCount++;
    
        if(this->loc.z > 15)
            this->active = false;
        this->loc.z += 0.5;
        angle += diff;
        if(angle > 30){
            diff = -5;
        }
        if(angle < -30){
            diff = +5;
        }
        
        glRotated(angle, 0, 0, 1);
    
    GFCameraWall::render();
}

void GFCameraWiggle::update(YTimeInterval dt){
    GFCameraWall::update(dt);
}

//------------------------------------------------------------------------
// name: GFBackingTrackProgressBar constructor
// desc: introduces the text object; render() handles the graphical progress bar,
//       update() computes time
//------------------------------------------------------------------------

GFBackingTrackProgressBar::GFBackingTrackProgressBar(){
    // initialize
    timestamp = new YText(1.0);
    timestamp->set("0:00");
    // move it to the right of the progress bar
    timestamp->loc.x = 3.55f;
    timestamp->loc.y = -2.3f;
    Globals::sim->root().addChild(timestamp);

    currentProgress = 7.0f*(elapsedTime / Globals::trackDuration)-3.5f;
}

void GFBackingTrackProgressBar::update(YTimeInterval dt) {
    
    elapsedTime += dt; // this is in seconds
    seconds = (int)(fmod(elapsedTime, 60.0f));
    minutes = (int)(elapsedTime / 60.0f);
    stringstream t;
    // convert time to string
    if (seconds < 10) {
//        t = SSTR(minutes << ":0" << seconds);
        t << std::to_string(minutes) << ":0" << seconds;
    } else {
//        t = SSTR(minutes << ":" << seconds);
        t<<minutes << ":" << seconds;
    }
    
    // set the displayed time in minutes and seconds
    timestamp->set(t.str());
}

void GFBackingTrackProgressBar::render() {

    // show progress bar below info bar
    
    currentProgress = 7.0f*(elapsedTime / Globals::trackDuration)-3.5f;
    glClearColor(0, 0, 0, 0);

    glEnable( GL_LIGHTING );

    glColor4f( 1.0f, 0.0f, 0.0f, 0.8f );

    glBegin( GL_QUADS );
    // show progress bar below info bar
    glVertex2f(-3.5f, -2.2f );
    glVertex2f(-3.5f, -2.3f );
    glVertex2f( currentProgress, -2.3f );
    glVertex2f( currentProgress, -2.2f );
    glEnd();
    
    glColor4f( 0.2f, 0.2f, 0.2f, alpha );
    
    glBegin( GL_QUADS );
    glVertex2f( currentProgress, -2.2f );
    glVertex2f( currentProgress, -2.3f );
    glVertex2f(3.5f , -2.3f );
    glVertex2f(3.5f , -2.2f );
    glEnd();
    
    // and text for timestamp (YText)
    
    glDisable( GL_LIGHTING );
}


//------------------------------------------------------------------------
// name: GFTextureWiggle constructor
// desc:
//------------------------------------------------------------------------

GFTextureWiggle::GFTextureWiggle(Mat img){
    glGenTextures(1, &texture);
    
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720,0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    Mat frameCopy;
    
    cv::flip( img, frameCopy, -1 );
    
    loadTexture_Mat(&frameCopy, &texture);
    GFTrackPlayer *player = new GFTrackPlayer("../../data/widdly.mp3");
    player->play();
            //imshow("ft", frameCopy);
}

void GFTextureWiggle::render(){
    frameCount++;
    if(this->loc.z > 15)
        this->active = false;
    this->loc.z += 0.02;
    angle += diff;
    if(angle > 30){
        diff = -5;
    }
    if(angle < -30){
        diff = +5;
    }
    
    glRotated(angle, 0, 0, 1);
    
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
    
    double ratio = 1280.0/720.0;
    double scale = 1.5;
    // camera wall is the farthest back thing, at -10.0
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,-3.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,-3.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,-3.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,-3.0);
    glEnd();
    glDisable( GL_TEXTURE_2D );
    
    

    
}

void GFTextureWiggle::update(YTimeInterval dt){
}

