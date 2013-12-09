//
//  main.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include <iostream>
#include "gf-gfx.h"
#include "gf-sim.h"
#include "gf-midi.h"
#include "gf-face.h"
#include "x-thread.h"
#include "gf-globals.h"\
// all hell will break loose with
// #include "gf-utils.h"


using namespace std;

int main( int argc, const char ** argv )
{
    //gf_init_face_rec();
    
    
    // invoke graphics setup and loop
    if( !gf_gfx_init( argc, argv ) )
    {
        // error message
        cerr << "Cannot initialize graphics/data system..." << endl;
        return -1;
    }    
    
    if( !gf_midi_init() )
    {
        // error message
        cerr << "Cannot initialize MIDI system..." << endl;
        //return -1;
    }
    
    /* if( !gf_audio_init() )
    {
        // error message
        cerr << "Cannot initialize MIDI system..." << endl;
    } */
    
    gf_init_cam_thread();
    
    cerr<<"done";
    // graphics loop
    gf_gfx_loop();

    return 0;
}
