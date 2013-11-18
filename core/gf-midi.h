//
//  gf-midi.h
//  GuitarFace
//
//  Created by Roshan Vid on 14/11/13.
//  Copyright (c) 2013 GuitarFace Inc. All rights reserved.
//

#ifndef __GuitarFace__gf_midi__
#define __GuitarFace__gf_midi__

#include<vector>
using namespace std;

int gf_midi_init();

class GFMIDIEvent{
    
public:
    GFMIDIEvent(int note_on, int pitch, int velocity, double delta_time);
    ~GFMIDIEvent();
    int getPitchClass(){ return m_pitch_class; }
    int getVelocity(){ return m_velocity; }
    bool isSimultaneous(){ return m_simultaneous; }
    void setSimultaneous(bool simultaneous);
    
    
private:
    int m_note_on;
    int m_midinote;
    int m_velocity;
    double m_delta_time;
    int m_pitch_class;
    bool m_simultaneous;
};

class GFNoteStore{
    
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
    long double getNotesPerHour(){ return m_notes_per_hour; }
    int * getPitchClasses(){ return m_pitch_classes; }
    
    
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
    int m_jump_count;
    int m_power_chord_count;
    int m_in_key_count;
    float m_notes_per_hour;
    int * m_pitch_classes;

    
    // not using these yet
    int m_vibrato_count;
    int m_bend_count;
    int m_pitch_bend_goal;
    int m_vibrato_goal;
    // to calculate simultaneity
    float time_thresh; // this should be global
    float velocity_thresh; // to avoid possible MIDI data glitchiness
    
};



#endif /* defined(__GuitarFace__gf_midi__) */
