//
//  gf-face.h
//  GuitarFace
//
//  Created by Roshan Vid on 25/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_face__
#define __GuitarFace__gf_face__

#include <iostream>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

#include "Tracker.h"

using namespace std;
using namespace cv;

class GFFaceRecognizer{
public:
    
    void detect();
    
    //Decides if its a guitarface, takes screenshot, does something cool
    void draw();
    void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                       CascadeClassifier& nestedCascade,
                       double scale, bool tryflip );
    
private:
    int getBlackPixelLevel();
    void detectBlackPixels( Mat& img );
    
};

int gf_init_face_rec();
void detectBlackPixels( Mat& img );

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                   CascadeClassifier& nestedCascade,
                   double scale, bool tryflip );
void Draw(cv::Mat &image,cv::Mat &shape,cv::Mat &con,cv::Mat &tri,cv::Mat &visi);
void ftDetect(Mat& img);

#endif /* defined(__GuitarFace__gf_face__) */
