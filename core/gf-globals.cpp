//-----------------------------------------------------------------------------
// name: gf-globals.h
// desc: global stuff for visquin visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "gf-globals.h"


// defaults
#define DEFAULT_FULLSCREEN    FALSE
#define DEFAULT_WINDOW_WIDTH  1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_BLENDSCREEN   FALSE
#define DEFAULT_FOG           FALSE
#define DEFAULT_VERSION       "1.0.0"

GFSim * Globals::sim = NULL;
GFNoteStore * Globals::data = NULL;

GLsizei Globals::windowWidth = DEFAULT_WINDOW_WIDTH;
GLsizei Globals::windowHeight = DEFAULT_WINDOW_HEIGHT;
GLsizei Globals::lastWindowWidth = Globals::windowWidth;
GLsizei Globals::lastWindowHeight = Globals::windowHeight;

SAMPLE * Globals::lastAudioBuffer = NULL;
SAMPLE * Globals::lastAudioBufferMono = NULL;
SAMPLE * Globals::audioBufferWindow = NULL;
unsigned int Globals::lastAudioBufferFrames = 0;
unsigned int Globals::lastAudioBufferChannels = 0;

GLboolean Globals::fullscreen = DEFAULT_FULLSCREEN;
GLboolean Globals::blendScreen = DEFAULT_BLENDSCREEN;

Vector3D Globals::blendAlpha( 1, 1, .5f );
GLfloat Globals::blendRed = 0.0f;
GLenum Globals::fillmode = GL_FILL;
iSlew3D Globals::bgColor( .5 );
Vector3D Globals::viewRadius( 5, 2, 1 );
Vector3D Globals::viewEyeY( 2, 0, 1.5f );
Vector3D Globals::fov( 80, 100, .2f );

GLuint Globals::textures[GF_MAX_TEXTURES];

GLfloat Globals::light0_pos[4] = { 2.0f, 1.2f, 4.0f, 1.0f };
GLfloat Globals::light1_ambient[4] = { .2f, .2f, .2f, 1.0f };
GLfloat Globals::light1_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Globals::light1_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Globals::light1_pos[4] = { -2.0f, 0.0f, -4.0f, 1.0f };
GLfloat Globals::linewidth = 1.0f;
GLboolean Globals::fog = DEFAULT_FOG;
GLuint Globals::fog_mode[4];
GLuint Globals::fog_filter;
GLfloat Globals::fog_density;

// colors
Vector3D Globals::ourWhite( 1, 1, 1 );
Vector3D Globals::ourRed( 1, .5, .5 );
Vector3D Globals::ourBlue( 102.0f/255, 204.0f/255, 1.0f );
Vector3D Globals::ourOrange( 1, .75, .25 );
Vector3D Globals::ourGreen( .7, 1, .45 );
Vector3D Globals::ourGray( .4, .4, .4 );
Vector3D Globals::ourYellow( 1, 1, .25 );
Vector3D Globals::ourSoftYellow( .7, .7, .1 );
Vector3D Globals::ourPurple( .6, .25, .6 );

std::string Globals::path = "";
std::string Globals::relpath = "data/texture/";
std::string Globals::datapath = "";
std::string Globals::version = DEFAULT_VERSION;

// midi keys
// add these to the root value, mod 12
int Globals::minor_key[7] = { 0, 2, 3, 5, 7, 8, 10 };
int Globals::major_key[7] = { 0, 2, 4, 5, 7, 9, 11 };
char * Globals::note_names[17] = { "A", "A#", "Ab", "B", "Bb", "C", "C#", "D", "Db",
     "D#", "E", "Eb", "F", "F#", "G", "G#", "Gb" };
char * Globals::interval_names[13] = { "P1", "m2", "M2", "m3", "M3", "P4", "TT", "P5",
    "m6", "M6", "m7", "M7", "P8" };

int Globals::root_number = 0;
string Globals::key_quality = "minor";
int Globals::note_goal = 500;
queue<GFMIDIEvent> Globals::note_queue;

GLfloat Globals::upperLipY;
GLfloat Globals::lowerLipY;
GLfloat Globals::mouthHeight;
GLfloat Globals::openMouthThresh = 0.1f;
GLboolean Globals::isMouthOpen;
