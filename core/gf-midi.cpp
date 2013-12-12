//-----------------------------------------------------------------------------
// name: do gf-midi.cpp
// desc: reads in midi data, computes things, sets the data structure "data'
//-----------------------------------------------------------------------------

#include "gf-midi.h"

using namespace std;

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

	// unsigned int bufferBytes = 0;
	// unsigned int bufferFrames = 512;
	
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
    m_notes = new int[m_note_goal];
    m_velocities = new int[m_note_goal];
    m_key = new int[m_key_size];
    m_time_stamps = new float[m_note_goal];
    m_beat_stamps = new float[m_note_goal]; 
     
     // these need to be "note_goal" in size because player should be able to
     // exceed these goals, even though they have their own goals, displayed
     // in the denominator. reaching note_goal will terminate the application.
     m_jumps = new bool[m_note_goal];
     m_pitch_bends = new bool[m_note_goal];
     m_vibrato = new bool[m_note_goal];
     m_power_chords = new int[m_note_goal];
     m_pace = new long double[m_note_goal];
     m_intervals = new int[m_note_goal];
     m_pitch_classes = new int[m_note_goal];
     // m_on_or_off = new bool[m_note_goal];
    
    
    
    m_note_on = note_on;
    m_pitch = pitch;
    m_velocity = vel;
    m_delta_time = t;
    
    // now, do the things associated with a note-on
    if( m_note_on == 144 )
    {
        // first things first: advance note_count
        Globals::data->m_note_count++;
        // reward every 50 notes!
        /*
        if (Globals::data->m_note_count == 100)
        {
            GFOverlayMessage *reward = new GFOverlayMessage("100_notes.png");
            reward->loc.z = -4;
            Globals::sim->root().addChild( reward );

        } else if (Globals::data->m_note_count == 200)
        {
            GFOverlayMessage *reward = new GFOverlayMessage("200_notes.png");
            reward->loc.z = -4;
            Globals::sim->root().addChild( reward );
        } else if (Globals::data->m_note_count == 300)
        {
            GFOverlayMessage *reward = new GFOverlayMessage("300_notes.png");
            reward->loc.z = -4;
            Globals::sim->root().addChild( reward );
        } */
        
        // store pitch in notes array, for a score
        m_notes[idx] = m_pitch;
        m_velocities[idx] = m_velocity;
        // compute pitch class
        m_pitch_class = pitch % 12;
        // count how many pitch classes we've done
        m_pitch_classes[m_pitch_class]++;
        // update pitch class histogram here?
        
        int interval;
        // determine intervals (P1, m2, etc.)
        if( idx != 0 )
        {
            interval = abs(m_notes[idx] - m_notes[idx-1]);
            m_intervals[idx] = interval;
            cout << "Interval: " << m_intervals[idx] << endl;
            
            // name the interval
            if( interval < 13 )
                m_interval_label[idx] = Globals::interval_names[interval];
            else
                m_interval_label[idx] = Globals::interval_names[interval%12];
        } else {
            m_intervals[idx] = 0; // first interval is 0, a P1...
            m_interval_label[idx] = "first_note";
        }
        
        // have to calculate time between note on/off
        
        // if not the first note, compute notes per hour
        // if( m_velocities[idx] != 0 && m_delta_time != 0.0f )
        // {
            // if (m_delta_time > 1.0f) Globals::data->m_notes_per_hour = 0.0f;
            Globals::data->m_notes_per_hour = (3600.0f/m_delta_time); // + Globals::data->m_notes_per_hour)/2.0f;
        // }
        
        
        // power chords
        if( idx > 2 )
        {
            if( m_velocities[idx] != 0 && m_delta_time < m_time_thresh ) // would prefer that this be based on note_on
            {
                if( m_intervals[idx] == 7 && m_delta_time_old >= m_time_thresh )
                    Globals::data->m_power_chord_count++;
                cout << "Simultaneity! Power chord count: " << Globals::data->m_power_chord_count << endl;
            }
        }
        
        
        // intonation
        for( int i = 0; i < m_key_size; i++ )
        {
            if( m_pitch_class == m_key[i] )
            {
                m_in_key_count++;
                break;
            }
        }
        
        // compute intervals
        // if greater than an octave, call it a "jump"
        if( m_intervals[idx] > 12 )
        {
            Globals::data->m_jump_count++;
            m_jumps[idx] = true;
            cout << "Jump! count: " << Globals::data->m_jump_count << endl;
            m_interval_label[idx] = Globals::interval_names[interval%12];
        }
        
        
        
        int n;
        // dynamic range is calculated from local velocities: need algorithm here (circular buffer)
        ( idx >= 10 ) ? n = 10 : n = idx;
        
        
        m_avg_velocity = 0.0f;
        for( int i = 0; i < n; i++ )
        {
            m_avg_velocity += m_velocities[idx-i]/(float)n;
        }
        if( m_velocity > m_max_velocity )
        {
            m_max_velocity = m_velocity;
        } else if( m_velocity < m_min_velocity )
        {
            m_min_velocity = m_velocity;
        }
        
        m_dynamic_range = m_max_velocity - m_min_velocity;
        
        idx++;
        
        
        // beat_stamps[idx] = delta_time;
        
    }
    
    // add the note to the data set
    Globals::data->appendNote(*this);
    
    // m_older_note_on = m_old_note_on;
    // m_old_note_on = m_note_on;
    
    m_delta_time_old = m_delta_time;

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
