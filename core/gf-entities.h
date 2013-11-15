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

#endif /* defined(__GuitarFace__gf_entities__) */
