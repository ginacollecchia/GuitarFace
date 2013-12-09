//
//  gf-entities.h
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_entities__
#define __GuitarFace__gf_entities__

#include "y-entity.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "gf-face.h"
#include "gf-globals.h"

//-----------------------------------------------------------------------------
// name: class GFTeapot
// desc: for testing
//-----------------------------------------------------------------------------
class GFTeapot : public YEntity
{
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
};

//-----------------------------------------------------------------------------
// name: class GFInfoBar
// desc: info bar
//-----------------------------------------------------------------------------
class GFInfoBar : public YEntity
{
public:
    GFInfoBar();
    ~GFInfoBar();
    YText *a;
        YText *b;
        YText *c;
        YText *d;
    void update( YTimeInterval dt);
    
    void render();
};

class GFPanel : public YEntity{
public:
    void update(YTimeInterval dt);

    void render();
};

//-----------------------------------------------------------------------------
// name: class GFOverlayMessage
// desc: cool overlay messages
//-----------------------------------------------------------------------------
class GFOverlayMessage : public YEntity
{
public:
    void update( YTimeInterval dt);
    
    void render();
    
    void setMessage(string _filename){ filename = _filename; };
    
    string getMessage(){ return filename; }
    
    GFOverlayMessage(string _filename);
    ~GFOverlayMessage();

private:
    XTexture *texture;
    static YTimeInterval deltatime;

    std::string filename;
};

//-----------------------------------------------------------------------------
// name: class GFTunnel
// desc: note tunnel
//-----------------------------------------------------------------------------
class GFTunnel : public YEntity
{
public:
    static YTimeInterval deltatime;
    void update(YTimeInterval dt);
    void render();
};

class GFTunnelLayer : public YEntity
{
public:
    void update(YTimeInterval dt);
    void render();
};

class GFNoteObject : public YEntity
{
public:
    int pitch;
    void update(YTimeInterval dt);
    void render();
};

class GFCameraWall : public YEntity {
private:
    int angle = 0;
    int diff = 5;
    int frameCount = 0;
    CvCapture *camCapture;
    GLuint texture;
public:
    void initCamera();
    void update(YTimeInterval dt);
    void render();
};

class GFVideoPlayer : public YEntity{
private:
    string file;
    cv::VideoCapture cam;
    GLuint texture;
public:
    GFVideoPlayer(string _file);
    ~GFVideoPlayer();
    void update(YTimeInterval dt);
    
    void render();
};

class GFBackgroundImage : public YEntity{
private:
    XTexture *texture;
    string filename;
public:
    void update(YTimeInterval dt);
    GFBackgroundImage(string filename);
    void render();
    
};

class GFGuitarFace : public YEntity {
private:
    cv::Mat image;
    GLuint texture;
    
public:
    // constructor
    GFGuitarFace(cv::Mat _image);
    ~GFGuitarFace();
    void update(YTimeInterval dt);
    void render();
};

#endif /* defined(__GuitarFace__gf_entities__) */