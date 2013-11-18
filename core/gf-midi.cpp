//-----------------------------------------------------------------------------
// name: do gf-midi.cpp
// desc: reads in midi data,
//-----------------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <math.h>
// #include "domidistuff.h"
#include "RtMidi.h"
#include "../api/rtaudio/RtAudio.h"
#include "gf-gfx.h"
#include "gf-globals.h"
#include "gf-midi.h"

using namespace std;

#define SAMPLE double
#define MY_FORMAT RTAUDIO_FLOAT64
#define MY_SRATE 44100
#define MIDI_MAX 127
#define MY_CHANNELS 1

//-----------------------------------------------------------------------------
// GOALS, CONSTANTS, RANGES
//-----------------------------------------------------------------------------
int note_goal = 100;
int pitch_bend_goal;
int dynamic_range_goal;
int jump_goal;
int vibrato_goal;
int key_size = 7;
float time_thresh;
float velocity_thresh;
int * key;

//-----------------------------------------------------------------------------
// RECORDS: arrays, counts, variables
//-----------------------------------------------------------------------------
// pitch data
int * notes;
int dynamic_range;
int note_count = 0;
// intonation
int in_key_count = 0;
// velocity data
int * velocities;
int min_velocity;
int max_velocity;
float avg_velocity;
// big skips in pitch
bool * jumps;
int jump_count = 0;
// pitch bends, slides
bool * pitch_bends;
int pitch_bend_count = 0;
// power chords
int * power_chords;
int power_chord_count = 0;
// notes per hour over time
float * pace;
float notes_per_hour;
// vibrato (oscillating pitch bends)
bool * vibrato;
int vibrato_count = 0;
// timing array (difference in timestamp)
float * beats;
int num_P1;
int num_m2;
int num_M2;
int num_m3;
int num_M3;
int num_P4;
int num_TT;
int num_P5;
int num_m6;
int num_M6;
int num_m7;
int num_M7;

int idx = 0;


//-----------------------------------------------------------------------------
// name: midiCallback
// desc: reads all the midi stuff
//-----------------------------------------------------------------------------
// RtMidiCallback data-type:
// typedef void (*RtMidiCallback)( double timeStamp, std::vector<unsigned char> *message, void *userData);
void midiCallback( double time_stamp, std::vector<unsigned char> *message, void *user_data )
{

    unsigned int nBytes = message->size();
    
	notes[idx] = (int)message->at(1);
	velocities[idx] = (int)message->at(2);

    GFMIDIEvent e((int)message->at(1),(int)message->at(2));
    
    Globals::data->appendNote(e);
    
    // dynamic range is calculated from local velocities
    if( idx > 10 )
    {
        avg_velocity = 0.0f;
        for( int i = 0; i < 10; i++ )
        {
            avg_velocity += velocities[idx-i]/10.0f;
        }
        if( velocities[idx] > max_velocity )
            max_velocity = velocities[idx];
        if( velocities[idx] < min_velocity )
            min_velocity = velocities[idx];
    }
    
    
    // print incoming message (uncomment for debugging)
    for ( unsigned int i = 0; i < nBytes; i++ )
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        std::cout << "stamp = " << time_stamp << std::endl;
	
    // if a NoteOn message is received, record data
    if ( nBytes > 0 && (int)message->at(0) == 144 )
    {
        // pitch stuff
        for( int i = 0; i < key_size; i++ )
        {
            if( notes[idx] == key[i] )
            {
                in_key_count++;
                break;
            }
        }
        
        // velocity stuff
        
        // duration stuff
        if( idx != 0 )
        {
            // difference in time between beats
            beats[idx] = time_stamp - beats[idx-1];
            // compute notes per hour
            note_count++;
            
        } else {
            beats[idx] = 0;
        }
    }
    
    idx++;
	
}

//-----------------------------------------------------------------------------
// name: setKey
//
//-----------------------------------------------------------------------------
void setKey( char root_letter, char * key_quality )
{
	
}

//-----------------------------------------------------------------------------
// name: main()
//
//-----------------------------------------------------------------------------
int gf_midi_init()
{
	RtMidiIn * midiin = NULL;
	RtAudio * audio = NULL;
	
	unsigned int bufferBytes = 0;
	
	unsigned int bufferFrames = 512;
	
    // MIDI config + init
    try
    {
        midiin = new RtMidiIn();
        notes = new int[note_goal];
        velocities = new int[note_goal];
        key = new int[key_size];
        beats = new float[note_goal];
        jumps = new bool[jump_goal];
        pitch_bends = new bool[pitch_bend_goal];
        vibrato = new bool[vibrato_goal];
        
    }
    catch( RtError & err ) {
        err.printMessage();
        goto cleanup;
    }
    
    // Check available ports.
    if ( midiin->getPortCount() == 0 )
    {
        std::cout << "No ports available!\n";
        goto cleanup;
    }
    // use the first available port
    midiin->openVirtualPort( "vport" );
    
    // set midi callback
    midiin->setCallback( &midiCallback );
    
    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );
	
	return 1;
    
cleanup:
	if(midiin)
		delete midiin;
	if(audio)
	{
		if( audio->isStreamOpen() )
			audio->closeStream();
		delete audio;
	}
	
	return 0;
	
}

GFMIDIEvent::GFMIDIEvent(int note, int vel): m_midinote(note), m_velocity(vel){
    //Calculate pitch & other stuff
}

GFMIDIEvent::~GFMIDIEvent(){
    
}

GFNoteStore::GFNoteStore(){

}

GFNoteStore::~GFNoteStore(){
    
}

void GFNoteStore::appendNote(GFMIDIEvent e){
    notes.push_back(e);
}


