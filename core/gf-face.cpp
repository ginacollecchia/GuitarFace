//
//  gf-face.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 25/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//


#include "gf-face.h"

string cascadeName = "../../data/haarcascade_frontalface_alt.xml";
string nestedCascadeName = "../../data/haarcascade_eye_tree_eyeglasses.xml";

int gf_init_face_rec()
{
    CvCapture* capture = 0;
    Mat frame, frameCopy, image;

    string inputName;
    bool tryflip = false;
    
    CascadeClassifier cascade, nestedCascade;
    double scale = 1;
    
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }
    
    capture = cvCaptureFromCAM(CV_CAP_ANY);
    
    cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );
    
    if( capture )
    {
        cout << "In capture ..." << endl;
        for(;;)
        {
            IplImage* iplImg;
            if((iplImg = cvQueryFrame( capture ))){
                frame = iplImg;
                if( frame.empty() )
                    break;
                if( iplImg->origin == IPL_ORIGIN_TL )
                    frame.copyTo( frameCopy );
                else
                    flip( frame, frameCopy, 0 );
                
                //detectAndDraw( frameCopy, cascade, nestedCascade, scale, tryflip );
                // detectBlackPixels(frameCopy);
            }
            
            
            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }
        
        waitKey(0);
        
    _cleanup_:
        cvReleaseCapture( &capture );
    }
    else{
        cerr<<"Couldn't capture from CAM."<<endl;
    }
    
    cvDestroyWindow("result");
    
    return 0;
}

void GFFaceRecognizer::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                   CascadeClassifier& nestedCascade,
                   double scale, bool tryflip )
{
    int i = 0;
    double t = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
    
    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );
    
    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
                             1.1, 2, 0
                             //|CV_HAAR_FIND_BIGGEST_OBJECT
                             //|CV_HAAR_DO_ROUGH_SEARCH
                             |CV_HAAR_SCALE_IMAGE
                             ,
                             Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CV_HAAR_FIND_BIGGEST_OBJECT
                                 //|CV_HAAR_DO_ROUGH_SEARCH
                                 |CV_HAAR_SCALE_IMAGE
                                 ,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        // crop image to the coordinates given by vector<Rect>faces, but just the bottom one third and the middle one third
        cv::Rect myROI( r->x+3+(r->width-6)/3.0, r->y+3+(r->height-6)*2.0/3.0, (r->width-6)/3.0, (r->height-6)/3.0 );
        cout << "x: " << r->x << " y: " << r->y << " width: " << r->width << " height: " << r->height << endl;
        
        // double aspect_ratio = (double)r->width/(double)r->height;
        // aspect_ratio = 0.6;
        // cout << "Width: " << r->width << " Height: " << r->height << endl;
        /* if( 1.1 < aspect_ratio && aspect_ratio < 1.25 )
        {
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else */
            rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
                      cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
                      color, 3, 8, 0);
        
        cout << myROI.width << endl;
        cv::Mat croppedImage = img(myROI);
        detectBlackPixels( croppedImage );

        
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
                                       1.1, 2, 0
                                       //|CV_HAAR_FIND_BIGGEST_OBJECT
                                       //|CV_HAAR_DO_ROUGH_SEARCH
                                       //|CV_HAAR_DO_CANNY_PRUNING
                                       |CV_HAAR_SCALE_IMAGE
                                       ,
                                       Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        
    }
    cv::imshow( "result", img );
    // IplImage* frame2 = cvCloneImage(&(IplImage)img);
    //         cvShowImage("result", frame2);
    
    
    
}

// maybe a better name would be detectBrownPixels, as the red does not shift as much as blue and green
void GFFaceRecognizer::detectBlackPixels( Mat& img ) {
    
    /// Load image
    // src = imread( argv[1], 1 );
    
    if( !img.data )
     return;
    
    cout<<"test";
    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( img, bgr_planes );
    
    /// Establish the number of bins
    int histSize = 256;
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    
    bool uniform = true; bool accumulate = false;
    
    Mat b_hist, g_hist, r_hist;
    
    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    
    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
             Scalar( 255, 0, 0), 2, 8, 0  );
        cout << "Blue: " << cvRound(b_hist.at<float>(i));
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
             Scalar( 0, 255, 0), 2, 8, 0  );
        cout << " Green: " << cvRound(g_hist.at<float>(i));
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
             Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
             Scalar( 0, 0, 255), 2, 8, 0  );
        cout << " Red: " << cvRound(r_hist.at<float>(i)) << endl;
    }
    
    /// Display
    namedWindow("calcHist Demo", WINDOW_AUTOSIZE );
    imshow("calcHist Demo", histImage );
    
    // waitKey(0);
}
