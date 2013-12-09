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
#include "RtAudio.h"
// #include "WvIn.h"


// static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData );

bool gf_audio_init();

class GFTrackPlayer {
public:
    void play();
    void pause();
    void loadFile(std::string filename);
    
private:
};

#endif /* defined(__GuitarFace__gf_utils__) */

class VideoPlayer {
public:
    void play();
    void pause();
    // void loadFile(string filename);
};

