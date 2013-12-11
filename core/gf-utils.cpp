//
//  gf-utils.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 20/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include "gf-utils.h"

GFTrackPlayer::GFTrackPlayer(string _filename):filename(_filename){
    int device = -1; // Default Sounddevice
    int freq = 44100; // Sample rate (Hz)
    // Handle for open stream
    
    /* Initialize output device */
    BASS_Init(device, freq, 0, 0, NULL);
    
    /* Load your soundfile and play it */
    streamHandle = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, 0);
    
}

GFTrackPlayer::~GFTrackPlayer(){
    BASS_Free();
}

void GFTrackPlayer::play(){    
    BASS_ChannelPlay(streamHandle, false);
    
    // get time stamp for progress bar
    
}

void GFTrackPlayer::pause(){
    BASS_ChannelPause(streamHandle);
}