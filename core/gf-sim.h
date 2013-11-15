//
//  gf-sim.h
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_sim__
#define __GuitarFace__gf_sim__

#include "gf-entities.h"
#include <iostream>

//-----------------------------------------------------------------------------
// name: class RAKASim
// desc: simulation class
//-----------------------------------------------------------------------------
class GFSim
{
public:
    GFSim();
    virtual ~GFSim();
    
public:
    // cascade timestep simulation through system (as connected to this)
    void systemCascade();
    
public:
    // pause the simulation
    void pause();
    // resume the simulation
    void resume();
    // get is paused
    bool isPaused() const;
    
public:
    // set desired frame rate
    void setDesiredFrameRate( double frate );
    // get it
    double getDesiredFrameRate() const;
    // get the timestep in effect (fixed or dynamic)
    YTimeInterval delta() const;
    
public:
    // get the root
    YEntity & root() { return m_gfxRoot; }
    
protected:
    YEntity m_gfxRoot;
    
public:
    double m_desiredFrameRate;
    bool m_useFixedTimeStep;
    
public:
    YTimeInterval m_timeLeftOver;
    YTimeInterval m_simTime;
    YTimeInterval m_lastDelta;
    bool m_first;
    bool m_isPaused;
};


#endif /* defined(__GuitarFace__gf_sim__) */










