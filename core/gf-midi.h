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
    GFMIDIEvent(int note, int velocity);
    ~GFMIDIEvent();
    string getPitch(){ return m_pitch;}
    int getVelocity(){return m_velocity;}
    bool isSimultaneous(){return m_simultaneous;}
    
private:
    int m_midinote;
    int m_velocity;
    string m_pitch;
    bool m_simultaneous;
};

class GFNoteStore{
public:
    GFNoteStore();
    ~GFNoteStore();
    void appendNote(GFMIDIEvent e);
    unsigned long getNoteCount(){
        return notes.size();
    }
private:
    vector<GFMIDIEvent> notes;
    int m_powerChordCount;
    int m_vibratoCount;
    int m_bendCount;
    
};



#endif /* defined(__GuitarFace__gf_midi__) */
