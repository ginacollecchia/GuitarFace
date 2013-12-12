//-----------------------------------------------------------------------------
// name: do gf-midi.cpp
// desc: reads in midi data, computes things, sets the data structure "data'
//-----------------------------------------------------------------------------

#include "gf-midi.h"

using namespace std;
int a = 2000;

int * m_notes = new int[a];
int * m_velocities = new int[a];
int * m_key = new int[7];
int * m_intervals = new int[a];
int * m_interval_label = new int[a];

//-----------------------------------------------------------------------------
// name: midiCallback
// desc: reads all the midi stuff
// RtMidiCallback data-type: void (*RtMidiCallback)( double timeStamp, std::vector<unsigned char> *message, void *userData);
//-----------------------------------------------------------------------------

void midiCallback( double delta_time, std::vector<unsigned char> *message, void *user_data )
{
    unsigned int nBytes = (int)message->size();
    
    // create a midi event
    GFMIDIEvent e( (int)message->at(0), (int)message->at(1), (int)message->at(2), delta_time );
    Globals::note_queue.push(e);
    
    // if the message is the right size and we have a note-on, record data
    if ( nBytes == 3 && (int)message->at(0)==144 )
    {
        
        // print incoming message (uncomment for debugging)
        for ( unsigned int i = 0; i < nBytes; i++ )
            std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
        if ( nBytes > 0 )
            std::cout << "stamp = " << delta_time << std::endl;
    }
    
    
    // global_time += delta_time;
    // time vector
    Globals::data->appendTime(delta_time);
    
}

//-----------------------------------------------------------------------------
// name: setKey
// desc: set the key from input
//-----------------------------------------------------------------------------
void GFMIDIEvent::setKey( int root_pcp, char * key_quality )
{
	if( strcmp(key_quality, "minor") != 0 )
    {
        for( int i = 0; i < sizeof(Globals::minor_key); i++ )
        {
            m_key[i] = (root_pcp + Globals::minor_key[i] ) % 12;
        }
    } else {
        for( int i = 0; i < sizeof(Globals::major_key); i++ )
        {
            m_key[i] = (root_pcp + Globals::major_key[i] ) % 12;
        }
    }
}

//-----------------------------------------------------------------------------
// name: gf_midi_init()
// desc: initialize MIDI stuff
//-----------------------------------------------------------------------------
int gf_midi_init()
{
	RtMidiIn * midiin = NULL;
	RtAudio * audio = NULL;
	
    // take user input: key, bpm, note_goal
    // setKey( argv[0], argv[1] );

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

//-------------------------------------------------------------------------------
// name: GFMidiEvent (constructor)
// desc: Calculate functions of pitch & velocity, i.e., do all MIDI work.
//-------------------------------------------------------------------------------

GFMIDIEvent::GFMIDIEvent( int note_on, int pitch, int vel, double t )
{
    
    m_note_on = note_on;
    m_pitch = pitch;
    m_velocity = vel;
    m_delta_time = t;
    
    // now, do the things associated with a note-on
    if( m_note_on == 144 && m_velocity > 0 )
    {
        // first things first: advance note_count
        Globals::data->m_note_count++;
        cout << "Note count: " << Globals::data->m_note_count << endl;
        // reward every 50 notes!
                
        // store pitch in notes array, for a score
        Globals::data->m_notes[Globals::idx] = m_pitch;
        Globals::data->m_velocities[Globals::idx] = m_velocity;
        Globals::data->m_time_stamps[Globals::idx] = m_delta_time;
        // compute pitch class
        m_pitch_class = m_pitch % 12;
        // count how many pitch classes we've done
        Globals::data->m_pitch_classes[m_pitch_class]++;
        // now we can update pitch class histogram
        
        int interval;
        // determine intervals (P1, m2, etc.)
        cout << Globals::idx << endl;
        if( Globals::idx > 0 )
        {
            interval = abs(Globals::data->m_notes[Globals::idx] - Globals::data->m_notes[Globals::idx-1]);
            Globals::data->m_intervals[Globals::idx] = interval;
            cout << "Interval: " << Globals::data->m_intervals[Globals::idx] << endl;
            
        }
        
        // have to calculate time between note on/off
        
        // compute notes per hour
        Globals::data->m_notes_per_hour = (3600.0f/m_delta_time);
        
        // power chords
        if( Globals::data->m_velocities[Globals::idx] != 0 && m_delta_time < m_time_thresh ) // would prefer that this be based on note_on, but the midi guitar doesn't send note offs
        {
            if( Globals::data->m_intervals[Globals::idx] == 7 )
            {
                Globals::data->m_power_chord_count++;
                // trigger a power chord graphical event
                Globals::data->m_power_chord = true;
            
                cout << "Simultaneity! Power chord count: " << Globals::data->m_power_chord_count << endl;
            } else {
                Globals::data->m_power_chord = false;
            }
        }
        
        
        // intonation
        for( int i = 0; i < m_key_size; i++ )
        {
            if( m_pitch_class == m_key[i] )
            {
                Globals::data->m_in_key_count++;
                break;
            }
        }
        
        // compute intervals
        // if greater than an octave, call it a "jump"
        if( Globals::data->m_intervals[Globals::idx] > 12 )
        {
            Globals::data->m_jump_count++;
            Globals::data->m_jumps[Globals::idx] = true;
            // cout << "Jump! count: " << Globals::data->m_jump_count << endl;
            // m_interval_label[idx] = Globals::interval_names[interval%12];
        }
        
        
        
        int n;
        // dynamic range is calculated from local velocities: need algorithm here (circular buffer)
        ( Globals::idx >= 10 ) ? n = 10 : n = Globals::idx;
        
        
        m_avg_velocity = 0.0f;
        for( int i = 0; i < n; i++ )
        {
            m_avg_velocity += Globals::data->m_velocities[Globals::idx-i]/(float)n;
        }
        if( m_velocity > m_max_velocity )
        {
            m_max_velocity = m_velocity;
        } else if( m_velocity < m_min_velocity )
        {
            m_min_velocity = m_velocity;
        }
        
        Globals::data->m_dynamic_range = m_max_velocity - m_min_velocity;
        
        // increase index number
        Globals::idx++;
        
    }
    
    // add the note to the data set
    Globals::data->appendNote(*this);
    
}

GFMIDIEvent::~GFMIDIEvent(){

}

//-------------------------------------------------------------------------------
// name: setSimultaneous
// desc: updates the simultaneous flag
//-------------------------------------------------------------------------------

void GFMIDIEvent::setSimultaneous(bool simultaneous) {
    m_simultaneous = simultaneous;
}


GFNoteStore::GFNoteStore(){

}

GFNoteStore::~GFNoteStore(){
    
}

//-------------------------------------------------------------------------------
// name: appendNote
// desc: add note to the queue
//-------------------------------------------------------------------------------

void GFNoteStore::appendNote(GFMIDIEvent e){
    notes.push_back(e);
}

//-------------------------------------------------------------------------------
// name: appendTime
// desc: record time stamp
//-------------------------------------------------------------------------------

void GFNoteStore::appendTime(double time) {
    times.push_back(time);
}

double GFNoteStore::getNotesPerHour() {
    return Globals::data->m_notes_per_hour;
}
