GuitarFace
==========

GuitarFace Wiki: https://ccrma.stanford.edu/wiki/GuitarFace

Goals

* To detect the facial features of the guitar face using OpenCV
* To make a visualizer of musical MIDI data that records and rewards user input in real time
* To be able to compare sessions against one another and track individual progress

MIDI code

We track via MIDI:
* duration of the session, number of notes played
* count of big jumps between pitch (adventurousness, comfort with pitch)
* count of pitches in the key, with key set by user at beginning
* count of notes on the beat, with tempo set by user at beginning and a metronome feature
* count of vibratos
* count of pitch bends
* count of slides
* count of power chords
* count of musical intervals (m2, M2, P4, etc.)
* chord labeling
* dynamic range (moving average / smart)
* pitch range
* stage presence, guitar face
* fretboard heatmap: where are you playing most frequently?
* repetition of pitch sequences
* clarity and consistency of dynamics and intonation
* mixture of divisions of the beat (i.e., all quarter notes = bad)
* pace of playing
* extra-musical moments, such as palm mutes and artificial harmonics, if detectable

These variables inform the graphical environment and analytic information.

