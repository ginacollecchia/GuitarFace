#include<iostream>
#include "domidistuff.cpp"


using namespace std;

int main(){
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
}