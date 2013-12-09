//
//  gf-gfx.h
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_gfx__
#define __GuitarFace__gf_gfx__

#include "x-def.h"
#include "x-gfx.h"
#include <string>


// entry point for graphics
bool gf_gfx_init( int argc, const char ** argv );
void gf_gfx_loop();
void gf_about();
void gf_endline();
void gf_line();
bool gf_initTexture( const std::string & filename, XTexture * tex );
void init_intro();
void init_game();
void init_report();

XTexture * gf_loadTexture( const std::string & filename );

#endif /* defined(__GuitarFace__gf_gfx__) */
