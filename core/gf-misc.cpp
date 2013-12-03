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

int loadTexture_Ipl(IplImage *image, GLuint *text) {
    
    if (image==NULL) return -1;
    
    glGenTextures(1, text);
    
	glBindTexture( GL_TEXTURE_2D, *text ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,0, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);
    return 0;
    
}

int loadTexture_Ipl2(IplImage *image, GLuint *text) {
    
    glGenTextures(1,text);
    glBindTexture(GL_TEXTURE_2D,*text);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,image->width,image->height,
                      GL_BGR,GL_UNSIGNED_BYTE,image->imageData);
    
    return 0;
}

//Use this only after glGenTextures and glTexImage2D
int loadTexture_Mat(cv::Mat *image, GLuint *text){
    if (image==NULL) return -1;
    
	glBindTexture( GL_TEXTURE_2D, *text ); //bind the texture to it's array
    std::cerr<<image->cols<<" "<<image->rows<<" "<<*text<<std::endl;
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->cols,image->rows, GL_BGR,GL_UNSIGNED_BYTE, image->data);
    return 0;
}
