// name: do midi stuff

#include <iostream>
#include <cstdlib>
#include <math.h>
// #include "domidistuff.h"
#include "RtMidi.h"
#include "../api/rtaudio/RtAudio.h"

using namespace std;

#define SAMPLE double
#define MY_FORMAT RTAUDIO_FLOAT64
#define MY_SRATE 44100
#define MIDI_MAX 127
#define MY_CHANNELS 1


// globals
int note_goal;
float time_thresh;
float velocity_thresh;
int pitch_bend_goal;
int dynamic_range_goal;
int min_velocity;
float avg_velocity;
int max_velocity;
int dynamic_range;
int root_note;

bool * jumps;
int note_count;
int * velocities;
bool * pitch_bends;
int * power_chords;
float * pace;
bool * vibrato;
int * key;
float * beats;
// beats[0] = 0;

int idx = 0;


//-----------------------------------------------------------------------------
// name: midiCallback
// desc: reads all the midi stuff
//-----------------------------------------------------------------------------
// RtMidiCallback data-type: 
// typedef void (*RtMidiCallback)( double timeStamp, std::vector<unsigned char> *message, void *userData);
void midiCallback( double time_stamp, std::vector< unsigned char > *message, void *user_data )
{
	unsigned int nBytes = message->size();
	int pitch = (int)message->at(1);
	int velocity = (int)message->at(2);
	
	if( velocity != 0 )
	{
		beats[idx+1] = time_stamp - beats[idx]; // difference in time between beats
		
	}
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
/* int main(int argc, char * argv[])
{
	RtMidiIn * midiin = NULL;
	RtAudio * audio = NULL;
	
	unsigned int bufferBytes = 0;
	
	unsigned int bufferFrames = 512;
	
    // MIDI config + init
    try 
    {
        midiin = new RtMidiIn();
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
    midiin->openPort( 0 );

    // set midi callback
    midiin->setCallback( &midiCallback );

    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes( false, false, false );
	
	
	
    // wait ...
    char input;
    cout << "Reading MIDI from port 0 ... press <enter> to quit" << endl;
    cin.get(input);
	
	
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
	
} */