GuitarFace
==========

GuitarFace Wiki (updated more often than this readme, at the present): https://ccrma.stanford.edu/wiki/GuitarFace

'''Goals'''

* To detect the facial features of the guitar face (mouth, eyes, proximity of features, angle of head) using OpenCV
* To make a visualizer of musical MIDI data that records and rewards user input in real time
* To be able to compare sessions against one another and track individual progress

'''MIDI code'''

We track via MIDI:
* duration of the session, number of notes played
* count of pitches in the user-defined key
* count of notes on or off the user-defined beats, e.g. tempo and meter (and an optional metronome feature)
* count of vibratos
* count of pitch bends
* count of slides
* count of power chords
* count of musical intervals (m2, M2, P4, etc.)
* count of "big jumps" between pitch (>P8)
* chord labeling
* dynamic range (moving average / smart)
* pitch range
* fretboard heat-map: where on the guitar neck are you playing most frequently?
* repetition of pitch sequences
* clarity and consistency of dynamics and intonation
* mixture of divisions of the beat (i.e., all quarter notes = bad)
* pace of playing
* extra-musical moments, such as palm mutes and artificial harmonics, if detectable
* the distribution of these features over the specific beats of a measure

These variables inform the graphical environment and analytic information.

