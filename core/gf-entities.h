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
    
    void setMessage();

private:
    std::string message;
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

#endif /* defined(__GuitarFace__gf_entities__) */