//
//  gf-entities.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include "gf-entities.h"

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


void GFInfoBar::render(){
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glBegin(GL_LINE_LOOP);
    glVertex2f(-3.5, -1.5);
    glVertex2f(-3.5, -1);
    glVertex2f(3.5, -1);
    glVertex2f(3.5, -1.5);
    glEnd();
    // disable lighting
    glDisable( GL_LIGHTING );
    
}

void GFInfoBar::update( YTimeInterval dt )
{
    // do nothing for now
}