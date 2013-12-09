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
                
                detectAndDraw( frameCopy, cascade, nestedCascade, scale, tryflip );
                 //detectBlackPixels(frameCopy);
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

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
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
    //printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        // crop image to the coordinates given by vector<Rect>faces, but just the bottom one third and the middle one third
        cv::Rect myROI( r->x+3+(r->width-6)/3.0, r->y+3+(r->height-6)*2.0/3.0, (r->width-6)/3.0, (r->height-6)/3.0 );
        //cout << "x: " << r->x << " y: " << r->y << " width: " << r->width << " height: " << r->height << endl;
        
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
        
        // cout << myROI.width << endl;
        cv::Mat croppedImage = img(myROI);

        //detectBlackPixels( croppedImage );
        double brightness = 0;
        getBrightness(croppedImage, brightness);
        
        cout<<brightness<<endl;
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
void detectBlackPixels( Mat& img ) {
    
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

// this is from FaceTracker; put this in a new thread (currently in graphics thread)
void Draw(cv::Mat &image, cv::Mat &shape, cv::Mat &con, cv::Mat &tri, cv::Mat &visi)
{
    int i,n = shape.rows/2; cv::Point p1,p2; cv::Scalar c;
    
    // draw triangulation
    c = CV_RGB(0,0,0);
    
    for(i = 0; i < tri.rows; i++){
        if(visi.at<int>(tri.at<int>(i,0),0) == 0 ||
           visi.at<int>(tri.at<int>(i,1),0) == 0 ||
           visi.at<int>(tri.at<int>(i,2),0) == 0)continue;
        p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
                       shape.at<double>(tri.at<int>(i,0)+n,0));
        p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
                       shape.at<double>(tri.at<int>(i,1)+n,0));
        cv::line(image,p1,p2,c);
        p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
                       shape.at<double>(tri.at<int>(i,0)+n,0));
        p2 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
                       shape.at<double>(tri.at<int>(i,2)+n,0));
        cv::line(image,p1,p2,c);
        p1 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
                       shape.at<double>(tri.at<int>(i,2)+n,0));
        p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
                       shape.at<double>(tri.at<int>(i,1)+n,0));
        cv::line(image,p1,p2,c);
    }
    // draw connections
    c = CV_RGB(0,0,255);
    for(i = 0; i < con.cols; i++){
        if(visi.at<int>(con.at<int>(0,i),0) == 0 ||
           visi.at<int>(con.at<int>(1,i),0) == 0)continue;
        p1 = cv::Point(shape.at<double>(con.at<int>(0,i),0),
                       shape.at<double>(con.at<int>(0,i)+n,0));
        p2 = cv::Point(shape.at<double>(con.at<int>(1,i),0),
                       shape.at<double>(con.at<int>(1,i)+n,0));
        cv::line(image,p1,p2,c,1);
    }
    // draw points
    // lips ~= 49 - 66
    for(i = 0; i < n; i++){
        if(visi.at<int>(i,0) == 0)continue;
        p1 = cv::Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
        c = CV_RGB(255,0,0); cv::circle(image,p1,2,c);
    }

    double upperLipY, lowerLipY, mouthHeight, openMouthThresh;
    
    // compute mouth height
    upperLipY = shape.at<double>(51+n,0); // i=51 is the middle of the exterior upper lip
    lowerLipY = shape.at<double>(57+n,0); // i=57 is the middle of the exterior lower lip
    mouthHeight = abs(upperLipY - lowerLipY);
    openMouthThresh = (float)image.rows / 14.5f; // 70.0 seems to be a good number, maybe on the high end, but won't detect mouth open if the subject is far away. could take running average and compare, but what's to say the average case isn't a bit of mouth open? it is relative to window height because face will be smaller with small window size.
    std::cout << "lip height: " << mouthHeight << endl;
    if (mouthHeight > openMouthThresh )
    {
        std::cout << "Mouth is open!" << endl;
        // do something in graphics, disable mouth detection for 4 seconds
        
        // wait?
        
        
        // only make a new guitar face if one is not currently visible
        if( !Globals::guitarFace )
        {
            Globals::guitarFace = true;
            Globals::guitar_face_time = 0;
            // GFGuitarFace *face = new GFGuitarFace(image);
            // face->Sphere( 1.0f );
        }

    } else {
        Globals::guitarFace = false;
    }

    /// Display
    namedWindow("face tracker mask", WINDOW_AUTOSIZE );
    imshow("face tracker mask", image );
    
    return;    

}

void ftDetect(Mat& im){

    bool fcheck = false;
    // double scale = 1;
    int fpd = -1;
    bool show = true;

    char ftFile[256] = "./data/face2.tracker";
    char triFile[256] = "./data/face.tri";
    char conFile[256] = "./data/face.con";
    
    // set other tracking parameters
    std::vector<int> wSize1(1); wSize1[0] = 7;
    std::vector<int> wSize2(3); wSize2[0] = 11; wSize2[1] = 9; wSize2[2] = 7;
    int nIter = 5; double clamp=3,fTol=0.01;
    FACETRACKER::Tracker model(ftFile);
    cv::Mat tri=FACETRACKER::IO::LoadTri(triFile);
    cv::Mat con=FACETRACKER::IO::LoadCon(conFile);

    cv::Mat gray;
    // double fps=0;
    // char sss[256];
    std::string text;

        bool failed = true;
    
    cv::flip(im,im,1); cv::cvtColor(im,gray,CV_BGR2GRAY);
    
    // track this image
    std::vector<int> wSize; if(failed)wSize = wSize2; else wSize = wSize1;
    if(model.Track(gray,wSize,fpd,nIter,clamp,fTol,fcheck) == 0){
        int idx = model._clm.GetViewIdx(); failed = false;
        Draw(im,model._shape,con,tri,model._clm._visi[idx]);
    }else{
        if(show){cv::Mat R(im,cvRect(0,0,150,50)); R = cv::Scalar(0,0,255);}
        model.FrameReset(); failed = true;
    }
    
}

void getBrightness(const cv::Mat& frame, double& brightness)
{
    cv::Mat temp, color[3], lum;
    temp = frame;
    
    split(temp, color);
    
    color[0] = color[0] * 0.299;
    color[1] = color[1] * 0.587;
    color[2] = color[2] * 0.114;
    
    
    lum = color[0] + color [1] + color[2];
    
    cv::Scalar summ = sum(lum);
    
    
    brightness = summ[0]/((::pow(2,8)-1)*frame.rows * frame.cols) * 2; //-- percentage conversion factor
}

void * camera( void *_this){
    cv::VideoCapture cam;
    
    cam.open(0);
    sleep(2);
    if(!cam.isOpened()){
        cout << "Failed opening video file." << endl;
    }
    
    std::cout << "Camera opened successfully" << std::endl;
    
    while(cam.get(CV_CAP_PROP_POS_AVI_RATIO) < 0.999999){
        Mat im; cam >> im;
        Globals::mutex.acquire();
        Globals::camQ.push(im);
        Globals::mutex.release();
        ftDetect(im);
        imshow("face tracker",im);
        waitKey(10);
    }
}

void gf_init_cam_thread(){
    XThread *t = new XThread();
    t->start(camera);
}
