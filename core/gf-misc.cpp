//
//  gf-misc.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 18/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include "gf-misc.h"

float degreesToRadian(float i){
    return 2 * M_PI * i;
}
void GLDrawEllipse (int segments, float width, float height, float x, float y , bool filled)
{
    glTranslatef(x, y, 0.0);

    float vertices[segments*2];
    int count=0;
    for (float i = 0; i < 360.0f; i+=(360.0f/segments))
    {
        std::cout<<"x:"<<vertices[count]<<" ";
        vertices[count++] = (cos(degreesToRadian(i))*width);
        std::cout<<"y:"<<vertices[count]<<" ";
        vertices[count++] = (sin(degreesToRadian(i))*height);
        std::cout<<std::endl;
    }
    glVertexPointer (2, GL_FLOAT , 0, vertices);
    glDrawArrays ((filled) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, segments);
}
void GLDrawCircle (int circleSegments, float circleSize, float x, float y, bool filled)
{
    GLDrawEllipse(circleSegments, circleSize, circleSize, x,y, filled);
}

void circle(float x, float y, float r, int segments)
{
    glBegin( GL_LINE_LOOP );
//    glVertex2f(x, y);
    for( int n = 0; n <= segments; ++n ) {
        float const t = 2*M_PI*(float)n/(float)segments;
        glVertex2f(x + sin(t)*r, y + cos(t)*r);
    }
    glEnd();
}