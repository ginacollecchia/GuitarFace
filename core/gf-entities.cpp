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
    
}
void GFInfoBar::render(){
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    YText *y = new YText(1.0);
    y->set("Notes Goal: " + to_string(Globals::data->getNoteCount())+ "/" + to_string(Globals::note_goal));
    y->loc.x = -3.4;
    y->loc.y = -1.8;
    this->addChild(y);

    y = new YText(1.0);
    y->set("NPH: " + to_string(Globals::data->getNotesPerHour()));
    y->loc.x = -1.7;
    y->loc.y = -1.8;
    this->addChild(y);
    
    y = new YText(1.0);
    y->set("Power Chords: " + to_string(Globals::data->getPowerChordCount()));
    y->loc.x = 0.0;
    y->loc.y = -1.8;
    this->addChild(y);

    y = new YText(1.0);
    y->set("Jumps: " + to_string(Globals::data->getJumpCount()));
    y->loc.x = 1.7;
    y->loc.y = -1.8;
    this->addChild(y);
    
    
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
    if(this->loc.z < -3){
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


