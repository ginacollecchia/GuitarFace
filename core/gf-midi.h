//
//  gf-midi.h
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_midi__
#define __GuitarFace__gf_midi__

#include <vector>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "RtMidi.h"
#include "RtAudio.h"
#include "gf-gfx.h"
#include "gf-globals.h"
#include "gf-entities.h"
using namespace std;

int gf_midi_init();

class GFMIDIEvent {
public:
    GFMIDIEvent(int note_on, int pitch, int velocity, double delta_time);
    ~GFMIDIEvent();
    int getPitchClass(){ return m_pitch_class; }
    // int getVelocity(){ return m_velocity; }
    bool isSimultaneous(){ return m_simultaneous; }
    void setSimultaneous( bool simultaneous );
    void setKey( int root_pcp, char * key_quality );

private:
    // the basic midi things
    int m_note_on;
    float m_delta_time;
    int m_velocity;
    int m_pitch;
    //-----------------------------------------------------------------------------
    // GOALS, CONSTANTS, RANGES, COUNTS
    //-----------------------------------------------------------------------------
    int m_note_goal = 500;
    int m_dynamic_range_goal;
    int m_jump_goal;
    int m_power_chord_goal;
    int * m_intervals;
    // intonation
    int m_in_key_count = 0;
    int m_key_size = 7;
    int * m_key;
    
    float m_time_thresh = 0.05f; // might need a second time thresh, if midi code is buggy
    float m_velocity_thresh; // if midi code is buggy
    double m_global_time = 0.0;
    
    //-----------------------------------------------------------------------------
    // RECORDS: arrays, counts, variables
    //-----------------------------------------------------------------------------
    // raw pitch data (12-big)
    int * m_notes;
    // store pitch class info in an array (also 12-big)
    int * m_pitch_classes;
    int m_pitch_class;
    int m_dynamic_range;
    // simultaneity stuff
    int m_old_note_on;
    int m_older_note_on;
    int m_num_simul_notes = 0;
    bool m_simultaneous;
    // velocity data
    int * m_velocities;
    int m_min_velocity;
    int m_max_velocity;
    float m_avg_velocity;
    // big skips in pitch
    bool * m_jumps = false;
    // pitch bends, slides
    bool * m_pitch_bends = false;
    // power chords
    int * m_power_chords = false;
    // current notes per whatever, though it should lag graphically
    long double * m_pace;
    // vibrato (oscillating pitch bends); can't do this with a midi guitar, but could take pitch wheel info
    bool * m_vibrato = false;
    int m_vibrato_count = 0;
    // timing array (difference in timestamp)
    float * m_time_stamps;
    float * m_beat_stamps;
    const char * m_interval_label[500];
    float m_delta_time_old;
    
    // indices
    int idx = 0;
};

class GFNoteStore {
    
public:
    GFNoteStore();
    ~GFNoteStore();
    void appendNote(GFMIDIEvent e);
    void appendTime(double time);
    // void getTime(){ return delta_time );
    
    // getter functions
    unsigned long getNoteCount(){ return notes.size(); }
    int getPowerChordCount(){ return m_power_chord_count; }
    int * getIntervals(){ return m_intervals; }
    int getJumpCount(){ return m_jump_count; }
    int getIntonationCount(){ return m_in_key_count; }
    double getNotesPerHour();
    int * getPitchClasses(){ return m_pitch_classes; }
    double m_notes_per_hour = 0.0f;
    int m_jump_count = 0;
    int m_power_chord_count = 0;
    int m_note_count = 0;

    
private:
    vector<GFMIDIEvent> notes;
    vector<double> times;
    // counts
    int m_key_size = 7;
    int * m_key;
    
    // goals: move these to another file?
    int m_note_goal = 500;
    int m_dynamic_range_goal;
    int m_jump_goal = 20;
    int m_power_chord_goal = 20;
    int * m_intervals;
    int m_in_key_count;
    int * m_pitch_classes;
    // store old delta time to detect if it's a power chord
    float m_delta_time_old;

    
    // not using these yet
    int m_vibrato_count;
    int m_bend_count;
    int m_pitch_bend_goal;
    int m_vibrato_goal;
    // to calculate simultaneity
    float time_thresh; // this should be global
    float velocity_thresh; // to avoid possible MIDI data glitchiness
    
};

// class GFPitchHistogram : public



#endif /* defined(__GuitarFace__gf_midi__) */
