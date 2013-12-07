//
//  gf-entities.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//
#include <iostream>
#include "gf-entities.h"
#include "gf-globals.h"
#include "gf-misc.h"

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
    s1<<"Notes Goal: "<<Globals::data->getNoteCount()<< "/"<<Globals::note_goal;
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

void GFTunnelLayer::update(YTimeInterval dt){
    
}

void GFTunnelLayer::render(){
    this->loc.z -= 0.01;
    circle(0, 0, 2, 12);
    if(this->loc.z < -4){
        this->active = false;
    }
    
}



void GFNoteObject::render(){
    this->loc.z -= 0.01;
    float deg = 2*M_PI*(float)pitch/(float)12;
    glTranslatef(2 * sin(deg), 2*cos(deg),1);
    glColor3f(1,0,0);
    glutSolidCube(0.2);
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
    
    /*
    double r = 3;
    int lats = 20;
    int longs = 12;
    int i, j;
    if(!Globals::note_queue.empty()){
        glutSolidCube(0.2);
    }
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);
        
        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        
        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            
                glColor4f(1.0, 0.0, 0.0, 1.0);

            glVertex3f( r * x * zr0,  r * y * zr0, 3);
            
            glColor4f(0.0, 0.0, 0.0, 1.0);
            glVertex3f(r * x * zr1, r * y * zr1, 3);
        }
        glEnd();
    }
 */
}

void GFCameraWall::initCamera(){
    
    int ret = 0;
    
    if (!(camCapture = cvCaptureFromCAM(CV_CAP_ANY))) {
        std::cout << "Failed to capture from camera" << std::endl;
        
        ret = 1;
    }
    
    glGenTextures(1, &texture);
    
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480,0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    

    std::cout << "Camera opened successfully" << std::endl;
}

void GFCameraWall::render(){

    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
    
    double ratio = 640.0/480.0;
    double scale = 2.0;
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
        
        //ftDetect(frameCopy);

        loadTexture_Mat(&frameCopy, &texture);
    }
}

GFVideoPlayer::GFVideoPlayer(string _file){
    file = _file;
    cam.open(file);
    
    if(!cam.isOpened()){
        std::cerr << "Failed opening video file." << endl;
    }
    
    glGenTextures(1, &texture);
    
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480,0, GL_BGR, GL_UNSIGNED_BYTE, 0);
    
    
    std::cerr << "Camera opened successfully" << std::endl;
}

GFVideoPlayer::~GFVideoPlayer(){
    
}

void GFCameraWall::update(YTimeInterval dt){

}

void GFVideoPlayer::render(){
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-2.0,-2.0,-3.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+2.0,-2.0,-3.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+2.0,+2.0,-3.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-2.0,+2.0,-3.0);    
    glEnd();
    glDisable(GL_TEXTURE_2D);
    Mat im;
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
    
//    double ratio = texture->origWidth/texture->origHeight;
//    double scale = 2.0;
//    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-(ratio*scale),-scale,-3.0); //with our vertices we have to assign a texcoord
//    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+(ratio*scale),-scale,-3.0); //so that our texture has some points to draw to
//    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+(ratio*scale),+scale,-3.0);
//    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-(ratio*scale),+scale,-3.0);
    
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-2.6666,-2.6666,0.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+2.6666,-2.6666,0.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+2.6666,+2.6666,0.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-2.6666,+2.6666,0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    this->loc.z += 0.08;
    if(this->loc.z > 1){
        this->active = false;
    }
}

GFBackgroundImage::GFBackgroundImage(string _filename):filename(_filename){
    string path = "/Users/roshanvid/Code/GuitarFace/build/Debug/data/texture/";
    texture = gf_loadTexture(path + filename);
}

void GFBackgroundImage::update(YTimeInterval dt){
}

void GFBackgroundImage::render(){
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
    glBindTexture( GL_TEXTURE_2D, texture->name); //bind the texture
    glBegin (GL_QUADS);    
    
    glTexCoord3d(0.0,0.0,-3.0); glVertex3d(-2.6666,-2.6666,0.0); //with our vertices we have to assign a texcoord
    glTexCoord3d(1.0,0.0,-3.0); glVertex3d(+2.6666,-2.6666,0.0); //so that our texture has some points to draw to
    glTexCoord3d(1.0,1.0,-3.0); glVertex3d(+2.6666,+2.6666,0.0);
    glTexCoord3d(0.0,1.0,-3.0); glVertex3d(-2.6666,+2.6666,0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


