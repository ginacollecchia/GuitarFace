//
//  gf-misc.h
//  GuitarFace
//
//  Created by Roshan Vid on 18/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_misc__
#define __GuitarFace__gf_misc__

#include "x-def.h"
#include <iostream>
#include "gf-gfx.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

float degreesToRadian(float i);
void GLDrawEllipse (int segments, float width, float height, float x, float y , bool filled);
void GLDrawCircle (int circleSegments, float circleSize, float x, float y, bool filled);
void circle(float x, float y, float r, int segments);
int loadTexture_Ipl(IplImage *image);
    
#endif /* defined(__GuitarFace__gf_misc__) */
