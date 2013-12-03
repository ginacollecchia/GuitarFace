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

  //  a->set("Notes Goal: " + to_string(Globals::data->getNoteCount())+ "/" + to_string(Globals::note_goal));
    a->loc.x = -3.4;
    a->loc.y = -1.8;
    

  //  b->set("NPH: " + to_string(Globals::data->getNotesPerHour()));
    b->loc.x = -1.7;
    b->loc.y = -1.8;
    
    
  //  c->set("Power Chords: " + to_string(Globals::data->getPowerChordCount()));
    c->loc.x = 0.0;
    c->loc.y = -1.8;

  //  d->set("Jumps: " + to_string(Globals::data->getJumpCount()));
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
    glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0); //with our vertices we have to assign a texcoord
    glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0); //so that our texture has some points to draw to
    glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
    glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
    glEnd();
    
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