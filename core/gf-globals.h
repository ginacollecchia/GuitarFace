//-----------------------------------------------------------------------------
// name: gf-globals.h
// desc: global stuff for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __GF_GLOBALS_H__
#define __GF_GLOBALS_H__


#include "x-def.h"
#include "x-audio.h"
#include "x-gfx.h"
#include "x-vector3d.h"
#include "gf-midi.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "x-thread.h"
// c++
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <queue>

// defines
#define GF_SRATE        44100
#define GF_FRAMESIZE    1024
#define GF_NUMCHANNELS  2
#define GF_MAX_TEXTURES 32
#define GF_MIDI_MAX     127




//-----------------------------------------------------------------------------
// name: enum BokehTextureNames
// desc: texture names
//-----------------------------------------------------------------------------
enum GuitarFaceTextureNames
{
    TUNNEL_BOKEH_NONE = 0,
    TUNNEL_BOKEH_1,
    TUNNEL_BOKEH_2,
    TUNNEL_BOKEH_3
};

enum BokehTextureNames
{
    RAKA_TEX_NONE = 0,
    RAKA_TEX_FLARE_TNG_1,
    RAKA_TEX_FLARE_TNG_2,
    RAKA_TEX_FLARE_TNG_3,
    RAKA_TEX_FLARE_TNG_4,
    RAKA_TEX_FLARE_TNG_5,
    RAKA_TEX_JELLY_BODY
};



// forward reference
class GFSim;




//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // top level root simulation
    static GFSim * sim;
    static GFNoteStore *data;
    
    // last audio buffer
    static SAMPLE * lastAudioBuffer;
    static SAMPLE * lastAudioBufferMono;
    static SAMPLE * audioBufferWindow;
    static unsigned int lastAudioBufferFrames;
    static unsigned int lastAudioBufferChannels;
    
    // width and height of the window
    static GLsizei windowWidth;
    static GLsizei windowHeight;
    static GLsizei lastWindowWidth;
    static GLsizei lastWindowHeight;
    
    // graphics fullscreen
    static GLboolean fullscreen;
    // blend pane instead of clearing screen
    static GLboolean blendScreen;
    // blend screen parameters
    static Vector3D blendAlpha;
    static GLfloat blendRed;
    // fill mode
    static GLenum fillmode;
    // background color
    static iSlew3D bgColor;
    // view stuff
    static Vector3D viewRadius;
    static Vector3D viewEyeY;
    static Vector3D fov;
    
    // textures
    static GLuint textures[];
    
    // light 0 position
    static GLfloat light0_pos[4];    
    // light 1 parameters
    static GLfloat light1_ambient[4];
    static GLfloat light1_diffuse[4];
    static GLfloat light1_specular[4];
    static GLfloat light1_pos[4];
    // line width
    static GLfloat linewidth;
    // do
    static GLboolean fog;
    static GLuint fog_mode[4];   // storage for three/four types of fog
    static GLuint fog_filter;    // which fog to use
    static GLfloat fog_density;  // fog density
    
    // colors
    static Vector3D ourWhite;
    static Vector3D ourRed;
    static Vector3D ourBlue;
    static Vector3D ourOrange;
    static Vector3D ourGreen;
    static Vector3D ourGray;
    static Vector3D ourYellow;
    static Vector3D ourSoftYellow;
    static Vector3D ourPurple;
    
    // path
    static std::string path;
    // path to datapath
    static std::string relpath;
    // datapath
    static std::string datapath;
    // version
    static std::string version;
    
    // midi stuff
    static int minor_key[7];
    static int major_key[7];
    // C, C#, etc.
    static char * note_names[17];
    // P1, m2, etc.
    static char * interval_names[13];
    static int root_number;
    static string key_quality;
    static int note_goal;
    static queue<GFMIDIEvent> note_queue;
    static queue<cv::Mat> camQ;
    static XMutex mutex;
    static double t_game_start;
    static double t_last_guitarface;
    static double d_guitarface_length;
    static double d_total_game_time;

    
    // face detection vars
    static GLfloat upperLipY;
    static GLfloat lowerLipY;
    static GLfloat mouthHeight;
    static GLfloat openMouthThresh;
    static GLboolean isMouthOpen;
    static GLboolean guitarFace;
    
    
};




#endif
