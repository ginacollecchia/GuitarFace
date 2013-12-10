//
//  gf-utils.h
//  GuitarFace
//
//  Created by Roshan Vid on 20/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_utils__
#define __GuitarFace__gf_utils__

#include <iostream>
#include "bass.h"

using namespace std;



class GFTrackPlayer {
public:
    GFTrackPlayer(string _filename);
    ~GFTrackPlayer();
    void play();
    void pause();
private:
    string filename;
    HSTREAM streamHandle;
};

class VideoPlayer {
public:
    void play();
    void pause();
    void loadFile(string filename);
};

#endif /* defined(__GuitarFace__gf_utils__) */