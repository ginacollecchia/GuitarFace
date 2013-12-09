//
//  gf-entities.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//
#include <iostream>
#include "gf-entities.h"

// bokehs
vector<YBokeh *> g_tunnels;


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
//    a->set("Notes Goal: " + to_string(Globals::data->getNoteCount())+ "/" + itoa(Globals::note_goal));
    this->addChild(a);
    b = new YText(1.0);
    this->addChild(b);
    c = new YText(1.0);
    this->addChild(c);
    d = new YText(1.0);
    this->addChild(d);
}
void GFInfoBar::render(){
    // enable lighting
    
    glEnable( GL_LIGHTING );
    // set color
    
    ostringstream s1;
    s1<<"Notes Goal: "<<Globals::data->getNoteCount(); // << "/"<<Globals::note_goal;
    a->set(s1.str());
    a->loc.x = -3.4;
    a->loc.y = -1.8;
    
    ostringstream s2;
    s2<<"NPH: " << Globals::data->getNotesPerHour();
    b->set(s2.str());
    b->loc.x = -1.7;
    b->loc.y = -1.8;

    ostringstream s3;
    s3<<"Power Chords: "<<Globals::data->getPowerChordCount();
    c->set(s3.str());
    c->loc.x = 0.0;
    c->loc.y = -1.8;

    ostringstream s4;
    s4<<"Jumps: " << Globals::data->getJumpCount();
    d->set(s4.str());
    d->loc.x = 1.7;
    d->loc.y = -1.8;

    
    
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glBegin(GL_LINE_LOOP);
    glVertex2f(-3.5, -2);
    glVertex2f(-3.5, -1.5);
    glVertex2f(3.5, -1.5);
    glVertex2f(3.5, -2);
    glEnd();
    // disable lighting
    glDisable( GL_LIGHTING );
    
}

void GFInfoBar::update( YTimeInterval dt )
{
        // do nothing for now
}

void GFTunnelLayer::update( YTimeInterval dt ){
    
}

void GFTunnelLayer::render(){
    this->loc.z -= 0.01;
    this->alpha -= 0.002;
    
    // void set( GLfloat _scale, GLfloat _alpha, GLfloat _scale_factor,
    //           GLfloat _alpha_factor, GLuint _texture );
    // void YBokeh::setBokehParams( GLfloat time, GLfloat freq, GLfloat time_step,
    //                              const Vector3D & xyz, const Vector3D & rgb )
    
    /* YBokeh *tunnel_ring = new YBokeh();
    tunnel_ring->set( 5.0f, alpha, 1.0f, 1.0f, TUNNEL_BOKEH_3 );
    tunnel_ring->sca.set(1.0f, 1.0f, 1.0f);
    // color is dependent on how many guitar faces have happened
    tunnel_ring->setBokehParams(1.0f, XFun::rand2f(0.1, 0.2), 1.0f, Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));
    tunnel_ring->setAlpha(alpha);
    this->addChild( tunnel_ring );
    g_tunnels.push_back( tunnel_ring ); */

    
    // glLineWidth( 15.0f );
    // glLineWidth( 1.0f );
    circle(0, 0, 2, 12);
    if(this->loc.z < -10){
        this->active = false;
    }
    
    
    /* // radius
    double r = 5;
    // number of rings
    int lats = 40;
    // number of vertices (pitch classes)
    int longs = 12;
    int i, j;
    // if(!Globals::note_queue.empty()){
    //     glutSolidCube(0.2);
    // }
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-1 + (double) (i-1) / lats);
        // double z0  = sin(lat0);
        double zr0 =  cos(lat0);
        
        double lat1 = M_PI * (-1 + (double) i / lats);
        // double z1 = sin(lat1);
        double zr1 = cos(lat1);
        
        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            
            glColor4f(0.0, 1.0, 0.0, this->alpha);
            
            // camerawall is at -10
            glVertex3f( r * x * zr0,  r * y * zr0, -1);
            
            glColor4f(0.0, 0.0, 0.0, this->alpha);
            glVertex3f(r * x * zr1, r * y * zr1, -1);
        }
        glEnd();
    } */

    
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
    float deg = 2*M_PI*(float)pitch/(float)12;
    glTranslatef(2 * sin(deg), 2*cos(deg),1);
    glColor3f(1,0,0);
    this->alpha -= 0.002;
    glutSolidCube(0.2);
    glDisable(GL_LIGHTING);
}

void GFNoteObject::update(YTimeInterval dt){
    
}

YTimeInterval GFTunnel::deltatime = 0;

void GFTunnel::update(YTimeInterval dt){

    deltatime += dt;
    if (deltatime>0.5){
        GFTunnelLayer *g = new GFTunnelLayer();
        g->loc.z = 2;
        this->addChild( g );
        deltatime = 0;
    }
}

void GFTunnel::render(){
        
    
    while(!Globals::note_queue.empty()){
        cout<<"event!"<<endl;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480,0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    
    std::cout << "Camera opened successfully" << std::endl;
    
}

GFCameraWall::~GFCameraWall(){
    
}


// Simple camera feed. No detection happens here. Look at gf_init_cam_thread in gf-face.h for face rec.

void GFCameraWall::render(){
    
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
    
    double ratio = 640.0/480.0;
    double scale = 3.0;
    // camera wall is the farthest back thing, at -10.0
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,-10.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,-10.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,-10.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,-10.0);
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
    cv::Mat im;
    cam >> im;
    
    loadTexture_Mat(&im, &texture);
}

void GFVideoPlayer::update(YTimeInterval dt){
    
}

GFOverlayMessage::GFOverlayMessage(string _filename):filename(_filename){
    string path = "/Users/roshanvid/Code/GuitarFace/build/Debug/data/texture/";    
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

    this->loc.z += 0.08;
    if(this->loc.z > 1){
        this->active = false;
    }
}

GFBackgroundImage::GFBackgroundImage(string _filename):filename(_filename){
    string path = "./data/texture/";
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

//
// Name :         CChildView::SphereFace()
// Description :  Draw a single facet of the sphere.  If p_recurse > 1,
//                triangulate that facet and recurse.
//

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

// Simple camera feed. No detection happens here. Look at gf_init_cam_thread in gf-face.h for face rec.

void GFCameraWiggle::render(){
    frameCount++;
    
        if(this->loc.z > 15)
            this->active = false;
        this->loc.z += 0.1;
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

