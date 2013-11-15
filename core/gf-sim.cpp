//
//  gf-sim.cpp
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#include "gf-sim.h"
using namespace std;

// up to max fps steps per second, e.g., 1./60
#define STEPTIME (1.0 / getDesiredFrameRate())
// max sim step size in seconds
#define SIM_SKIP_TIME (.25)

//-------------------------------------------------------------------------------
// name: GFSim()
// desc: constructor
//-------------------------------------------------------------------------------
GFSim::GFSim()
{
    m_desiredFrameRate = 60;
    m_useFixedTimeStep = false;
    m_timeLeftOver = 0;
    m_simTime = 0;
    m_lastDelta = 0;
    m_first = true;
    m_isPaused = false;
}

//-------------------------------------------------------------------------------
// name: ~GFSim()
// desc: ...
//-------------------------------------------------------------------------------
GFSim::~GFSim()
{
    // nothing to do
}

//-------------------------------------------------------------------------------
// name: systemCascade()
// desc: trigger system wide update with time steps
//-------------------------------------------------------------------------------
void GFSim::systemCascade()
{
    // get current time (once per frame)
    XGfx::getCurrentTime( true );
    
    // Timing loop
    YTimeInterval timeElapsed = XGfx::getCurrentTime() - m_simTime;
    m_simTime += timeElapsed;
    
    // special case: first update
    if( m_first )
    {
        // set time just enough for one update
        timeElapsed = STEPTIME;
        // set flag
        m_first = false;
    }
    
    // clamp it
    if( timeElapsed > SIM_SKIP_TIME )
        timeElapsed = SIM_SKIP_TIME;
    
    // update it
    // check paused
    if( !m_isPaused )
    {
        // update the world with a fixed timestep
        m_gfxRoot.updateAll( timeElapsed );
    }
    
    // redraw
    m_gfxRoot.drawAll();
    
    // set
    m_lastDelta = timeElapsed;
}



//-------------------------------------------------------------------------------
// pause the simulation
//-------------------------------------------------------------------------------
void GFSim::pause() { m_isPaused = true; }
//-------------------------------------------------------------------------------
// resume the simulation
//-------------------------------------------------------------------------------
void GFSim::resume() { m_isPaused = false; }
//-------------------------------------------------------------------------------
// get is paused
//-------------------------------------------------------------------------------
bool GFSim::isPaused() const { return m_isPaused; }
//-------------------------------------------------------------------------------
// set desired frame rate
//-------------------------------------------------------------------------------
void GFSim::setDesiredFrameRate( double frate ) { m_desiredFrameRate = frate; }
//-------------------------------------------------------------------------------
// get it
//-------------------------------------------------------------------------------
double GFSim::getDesiredFrameRate() const { return m_desiredFrameRate; }
//-------------------------------------------------------------------------------
// get the timestep in effect (fixed or dynamic)
//-------------------------------------------------------------------------------
YTimeInterval GFSim::delta() const
{ return m_lastDelta; }
