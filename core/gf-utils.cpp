//
//  gf-utils.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 20/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include "gf-utils.h"

const char * mus_file_names = "./data/sndfiles/baba.wav";
// stk::FileWvIn * g_input_music;

// does not like STK

//-----------------------------------------------------------------------------
// name: gf_audio_init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
/* bool gf_audio_init()
{
    cout << "Initializing audio..." << endl;
    unsigned int frameSize = 512;
    // initialize
    if( !XAudioIO::init( 0, 0, 44100, frameSize , 2, audio_callback, NULL ) )
    {
        // done
        return false;
    }
    
    // Reading in Wave file
    g_input_music = new stk::FileWvIn(mus_file_names, 0, 0);
    g_input_music->normalize(1);
    
    return true;
} */