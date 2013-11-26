/*
 #include <iostream>

#include <GL/glew.h>        // Include OpenGL headers
#include <GL/glfw.h>        // Include OpenGL Framework headers

#include "opencv/cv.h"      // Include OpenCV
#include "opencv/highgui.h" // Include OpenCV highgui

using namespace std;

// Vars for the size of our OpenGL window
float windowWidth  = 640.0f;
float windowHeight = 480.0f;

int sliderPosition = 9;    // Initial slider position
int divisions = 32;        // Initial number of divisions

uint blockXSize;           // Horizontal block size
uint blockYSize;           // Vertical block size

uint pixelCount;           // The number of pixels in a block (blockXSize multiplied by blockYSize)

uint camWidth;             // The width  of the webcam input stream
uint camHeight;            // The height of the webcam input stream

bool quit = false;         // Enough already!

bool useLines     = false;       // If true use lines, otherwise use points
bool smoothPoints = false;       // Whether to smooth the points or not. Unsmoothed points gives squares/rectangles - which looks quite cool!
bool sizeFromBrightness = false; // Whether to use the average brightness of a block to alter the point size

// Template function to convert things to strings
template<class T>
std::string toString(const T& t)
{
    std::ostringstream stream;
    stream << t;
    return stream.str();
}

// Structure to hold a colour as RGB values in the range 0 to 255
struct Colour3ub
{
    uint8_t redComponent;
    uint8_t greenComponent;
    uint8_t blueComponent;
};


// Function to handle screen resizing
void resize(int theWidth, int theHeight)
{
    // Resize the window
    glfwSetWindowSize(theWidth, theHeight);
    
    // Set our global window size variables to their new value
    windowWidth = theWidth;
    windowHeight = theHeight;
    
    // Setup our viewport to be the entire size of the window
    glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
    
    // Change to the projection matrix and set orthagonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, 0, 1); // Paramters: left, right, bottom, top, near, far
    
    // Make sure we're changing the model view and not the projection then reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// Handle keypresses and respond accordingly
void handleKeypress(int theKey, int theAction)
{
	// If a key was pressed...
	if (theAction == GLFW_PRESS)
	{
	    // ...react accordingly!
	    switch (theKey)
	    {
                // Use L to switch to drawing lines
            case 'L':
                useLines = true;
                break;
                
                // Use P to switch to drawing points
            case 'P':
                useLines = false;
                break;
                
                // Use S to switch whether to generate the point size from the overall block brightness or not
            case 'S':
                sizeFromBrightness = !sizeFromBrightness;
                break;
                
                // Use Space to toggle point smoothing (on gives you circular points, off gives you square points)
            case GLFW_KEY_SPACE:
                if (smoothPoints == false)
                {
                    glEnable(GL_POINT_SMOOTH);
                    smoothPoints = true;
                }
                else
                {
                    glDisable(GL_POINT_SMOOTH);
                    smoothPoints = false;
                }
                break;
                
                // Use Escape to quit
            case GLFW_KEY_ESC:
                quit = true;
                break;
                
            default:
                break;
                
        } // End of switch statement
        
	} // End of if keyaction was GLFW_PRESS condition
    
} // End of handleKeypress function


void initGL(int theWidth, int theHeight, string theWindowTitle)
{
    //  ----- GLFW Setup -----
    
    glfwInit();
    
    // Frame counter and window settings variables
    int	redBits    = 8, greenBits = 8,   blueBits   = 8;
    int alphaBits  = 8, depthBits = 0,  stencilBits = 0;
    
    // Create a window or die trying...
    if(!glfwOpenWindow(windowWidth, windowHeight, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW))
    {
        cout << "Failed to open window!" << endl;
        glfwTerminate();
        exit(-1);
    }
    
    // Register the callback to change the window size
    glfwSetWindowSizeCallback(&resize);
    
    // Register the callback to handle keypresses
    glfwSetKeyCallback(handleKeypress);
    
    
    //  ----- GLEW Setup -----
    
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "GLEW initialisation error: " << glewGetErrorString(err) << endl;
        exit(-1);
    }
    cout << "GLEW intialised successfully. Using GLEW version: " << glewGetString(GLEW_VERSION) << endl;
    
    // ----- Viewport and Projection Setup -----
    
    // Setup our viewport to be the entire size of the window
    glViewport(0, 0, (GLsizei)theWidth, (GLsizei)theHeight);
    
    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, 0, 1); // Paramters: left, right, bottom, top, near, far
    
    // Make sure we're changing the model view and not the projection then reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // ----- OpenGL Setup -----
    
    glfwSwapInterval(1); 		// Lock to vertical sync of monitor (normally 60Hz, so 60fps)
    
    glDisable(GL_SMOOTH);		// Disable (gouraud) shading
    
    glDisable(GL_DEPTH_TEST); 	// Disable depth testing
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // // Set our clear colour to black
    
    glEnable(GL_LINE_SMOOTH);	// Enable anti-aliasing on lines
    
}

void calcFPS(uint & frame, double & t0, double & time)
{
    //get the current time
    time = glfwGetTime();
    
    // Calculate and display the FPS
    if((time-t0) > 1.0 || frame == 0)
    {
        double fps = (double)frame / (time - t0);
        
        string windowTitle = "Webcam OpenGL Manipulation | FPS: ";
        
        string tempFPS = toString(fps);
        
        windowTitle += tempFPS;
        
        const char* pWindowTitle = windowTitle.c_str();
        
        glfwSetWindowTitle(pWindowTitle);
        
        t0 = time;
        frame = 0;
    }
    
    frame ++; // Increment our frame count
    
}


// Callback function to adjust number of divisions when we move the slider
void onDivisionSlide(int theSliderValue)
{
    // Put a lower limit of 1 on our slider position
    if (sliderPosition < 0)
    {
        sliderPosition = 1;
    }
    
    // Set the number of divisions depending on the slider location
    // Factors of both 640 and 480: 1, 2, 4, 5, 8, 10, 16, 20, 32, 40, 160
    switch (theSliderValue)
    {
        case 1:
            divisions = 1;
            break;
            
        case 2:
            divisions = 2;
            break;
            
        case 3:
            divisions = 4;
            break;
            
        case 4:
            divisions = 5;
            break;
            
        case 5:
            divisions = 8;
            break;
            
        case 6:
            divisions = 10;
            break;
            
        case 7:
            divisions = 16;
            break;
            
        case 8:
            divisions = 20;
            break;
            
        case 9:
            divisions = 32;
            break;
            
        case 10:
            divisions = 40;
            break;
            
        case 11:
            divisions = 160;
            break;
            
        default:
            break;
    }
    
    // Recalculate our block sizes and pixelCount for the new number of divisions
    blockXSize = camWidth  / divisions;
    blockYSize = camHeight / divisions;
    
    pixelCount = blockXSize * blockYSize; // Calculate our new pixelCount
}


// Function to draw a point at a specified location with a specified colour
void glDrawPoint (const GLfloat & xLoc, const GLfloat & yLoc, const Colour3ub & theColour)
{
    // Set the colour
    glColor3ub(theColour.redComponent, theColour.greenComponent, theColour.blueComponent);
    
    // Set the point size based on the brightness of the colour divided by a fudge-factor
    float averageColour = (theColour.redComponent + theColour.greenComponent + theColour.blueComponent) / 3.0f;
    
    if (sizeFromBrightness == true)
    {
    	glPointSize(averageColour / 15.0f); // Divide by lower numbers for wacky 3D-esque effects!
    }
    else
    {
    	glPointSize(blockYSize);
    }
    
    // Draw the point
    glBegin(GL_POINTS);
	glVertex2f(xLoc, yLoc);
    glEnd();
    
}


// Function to draw a line between two specified points with a specified colour
void glDrawLine (const GLfloat & xLoc, const GLfloat & yLoc, const GLfloat & xLoc2, const GLfloat & yLoc2, const Colour3ub & theColour)
{
    // Set the colour
    glColor3ub(theColour.redComponent, theColour.greenComponent, theColour.blueComponent);
    
    // Set the line width based on the brightness of the colour divided by a fudge-factor
    float averageColour = (theColour.redComponent + theColour.greenComponent + theColour.blueComponent) / 3.0f;
    
    glLineWidth(averageColour / 15.0f);
    
    // Draw a line between the top-left and bottom-right points of the block
    glBegin(GL_LINES);
    glVertex2f(xLoc, yLoc);
	glVertex2f(xLoc2, yLoc2);
    glEnd();
}


// Function to calculate the average colour of a block of pixels
Colour3ub getBlockColour(IplImage * pFrame, const uint & xLoop, const uint & yLoop, const uint & pixelsPerBlock)
{
    // Create and initialise colour component counters
    uint redSum     = 0;
    uint greenSum   = 0;
    uint blueSum    = 0;
    
    uchar * ptr; // Create our pointer to the pixel beforehand to avoid running the constructor each time through the loops
    
    // Read every pixel in the block and calculate the average colour
    for (uint pixXLoop = 0; pixXLoop < blockXSize; pixXLoop++)
    {
        for (uint pixYLoop = 0; pixYLoop < blockYSize; pixYLoop++)
        {
            
            // Get the pixel colour from the webcam stream
            ptr = cvPtr2D(pFrame, yLoop + pixYLoop, xLoop + pixXLoop, NULL);
            
            // Add each component to its sum
            redSum   += ptr[2];
            greenSum += ptr[1];
            blueSum  += ptr[0];
            
        } // End of inner y pixel counting loop
        
    } // End of outer x pixel countier loop
    
    // Calculate the average colour of the block
    Colour3ub colour;
    colour.redComponent   = redSum   / pixelsPerBlock;
    colour.greenComponent = greenSum / pixelsPerBlock;
    colour.blueComponent  = blueSum  / pixelsPerBlock;
    
    return colour;
}


int main()
{
    // Frame-rate calculator var declaration and initialisation
    double time, t0;
    uint frame = 0;
    t0 = glfwGetTime();
    
    // Call our initGL function to set up our OpenGL options
    string windowTitle = "Low Rez Stream Circles | r3dux.org\tFPS: --";
    initGL(windowWidth, windowHeight, windowTitle);
    
    // Create two OpenCV windows
    cvNamedWindow("WebCam", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Low Rez Stream", CV_WINDOW_AUTOSIZE);
    
    int maxSliderValue = 11;
    
    // Create the divisions slider lider
    cvCreateTrackbar("Division Setting", "Low Rez Stream", &sliderPosition, maxSliderValue, onDivisionSlide);
    
    // Start capturing data from the web cam
    CvCapture* pCapture = cvCaptureFromCAM(CV_CAP_ANY);
    
    // Get an initial frame so we know the size of things (cvQueryFrame is a combination of cvGrabFrame and cvRetrieveFrame)
    IplImage* pFrame = NULL;
    
    
    pFrame = cvQueryFrame(pCapture);
        
    // Create an image the same size and colour-depth as our input stream
    IplImage* pLowRezFrame = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);
    
    // Get the width and height of our webcam input stream
    camWidth  = pFrame->width;
    camHeight = pFrame->height;
    
    // Keep track of how many pixels in a block (so we can divide by it)
    uint pixelsPerBlock = 0;
    
    // Loop controling vars
    char keypress;
    
    while (quit == false)
    {
        // Clear the screen and reset the modelview matrix
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        
        // Grab a frame from the webcam
        pFrame = cvQueryFrame(pCapture);
        
        // Draw the original frame and low resolution version
        cvShowImage("WebCam", pFrame);
        cvShowImage("Low Rez Stream", pLowRezFrame);
        
        // Calculate our blocksize per frame to cater for slider
        blockXSize = camWidth  / divisions;
        blockYSize = camHeight / divisions;
        
        // Translate across and down a little to put our drawn geometry in the centre of the blocks
        glTranslatef( (float)(blockXSize / 2), (float)(blockYSize / 2), 0.0f);
        
        pixelsPerBlock = blockXSize * blockYSize; // How many pixels we'll read per block - used to find the average colour
        
        // Uncomment to get your block-stat fix...
        //cout << "At " << divisions << " divisions (Block size " << blockXSize << "x" << blockYSize << ", so " << pixelCount << " pixels per block)" << endl;
        
        // Loop through each block horizontally
        for (uint xLoop = 0; xLoop < camWidth; xLoop += blockXSize)
        {
            
            // Loop through each block vertically
            for (uint yLoop = 0; yLoop < camHeight; yLoop += blockYSize)
            {
                // Get the colour of the block
                Colour3ub colour = getBlockColour(pFrame, xLoop, yLoop, pixelsPerBlock);
                
                // Draw a rectangle of the average colour
                cvRectangle(
                            pLowRezFrame,
                            cvPoint(xLoop, yLoop),
                            cvPoint(xLoop + blockXSize, yLoop + blockYSize),
                            CV_RGB(colour.redComponent, colour.greenComponent, colour.blueComponent),
                            CV_FILLED,
                            8,
                            0
                            );
                
                if (useLines == true)
                {
                    glDrawLine(xLoop, yLoop, xLoop + blockXSize, yLoop + blockYSize, colour);
                }
                else
                {
                    glDrawPoint(xLoop, yLoop, colour);
                }
                
            } // End of inner y loop
            
        } // End of outer x loop
        
        // Swap the buffers
        glfwSwapBuffers();
        
        // Calculate the FPS
        calcFPS(frame, t0, time);
        
        // Wait 2 milliseconds - if we don't wait at least 2ms then we're polling faster than the webcam and although
        // the stream will still be captured and shown in the OpenGL window, it will not be displayed in any OpenCV windows.
        keypress = cvWaitKey(2);
        
    } // End of while loop
    
    // Release our stream capture object
    cvReleaseCapture(&pCapture);
    
    // Release our images and destroy all windows
    cvReleaseImage(&pFrame);
    cvReleaseImage(&pLowRezFrame);
    cvDestroyAllWindows();
}
*/
/*
#include <iostream>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

#define CAMERA_OUTPUT_WINDOW_NAME "camera-output"

int main(int argc, char **argv)
{
    CvCapture *camCapture;
    int ret = 0;
    
    if (!(camCapture = cvCaptureFromCAM(CV_CAP_ANY))) {
        cout << "Failed to capture from camera" << endl;
        
        ret = 1;
        
        goto exitCameraOpenFailed;
    }
    
    cout << "Camera opened successfully" << endl;
    
    cvNamedWindow(CAMERA_OUTPUT_WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    
    IplImage *cameraFrame;
    int grabFrameRet;
    
    while (true) {
        if ((cameraFrame = cvQueryFrame(camCapture))) {
            cvShowImage(CAMERA_OUTPUT_WINDOW_NAME, cameraFrame);
        }
        
        if (cvWaitKey(60) != -1) {
            cout << "Input" << endl;
            break;
        }
    }
    
    cout << "Done" << endl;
    
    cvReleaseCapture(&camCapture);
    cvDestroyWindow(CAMERA_OUTPUT_WINDOW_NAME);
exitCameraOpenFailed:
    return ret;
}
*/

/*
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <ctype.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "x-thread.h"
#include <queue>

//using namespace std;


GLuint texture; //the array for our texture
std::queue<IplImage> camImages;
IplImage *imagenText1;
XMutex mutex;


//The IplImage to OpenGl texture function
int loadTexture_Ipl(IplImage *image, GLuint *text) {
    
    if (image==NULL) return -1;
    
    glGenTextures(1, text);
    
	glBindTexture( GL_TEXTURE_2D, *text ); //bind the texture to it's array
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,0, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);
    return 0;
    
}

void display (void) {
	
	glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glEnable( GL_TEXTURE_2D ); //enable 2D texturing
	glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture
    glBegin (GL_QUADS);
    glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0); //with our vertices we have to assign a texcoord
    glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0); //so that our texture has some points to draw to
    glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
    glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
    glEnd();
    glutSwapBuffers();
}

void FreeTexture( GLuint texture )
{
    glDeleteTextures( 1, &texture );
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

void * do_something( void *_this){
    CvCapture *camCapture;
    int ret = 0;
    
    if (!(camCapture = cvCaptureFromCAM(CV_CAP_ANY))) {
        std::cout << "Failed to capture from camera" << std::endl;
        
        ret = 1;
        
        goto exitCameraOpenFailed;
    }
    
    std::cout << "Camera opened successfully" << std::endl;
    
    
    IplImage *cameraFrame;
    
    while (true) {
        if ((cameraFrame = cvQueryFrame(camCapture))) {
            IplImage img;            
            memcpy(&img, cameraFrame, sizeof(img));
            mutex.acquire();
            camImages.push(img);
            mutex.release();
            std::cout<<"writing image";
        }
        
        if (cvWaitKey(60) != -1) {
            std::cout << "Input" << std::endl;
            break;
        }
    }
    
    std::cout << "Done" << std::endl;
    
    cvReleaseCapture(&camCapture);
exitCameraOpenFailed:

    return NULL;
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("A basic OpenGL Window");
	glutDisplayFunc (display);
    glutIdleFunc (display);
    glutReshapeFunc (reshape);
    
	imagenText1=cvLoadImage("lena.jpg");
    
    XThread *t = new XThread();
    
    t->start(do_something);
    
	//The load iplimage to opengl texture
    
//	while(true){
//        mutex.acquire();
//        std::cout<<camImages.size();
//        loadTexture_Ipl( &camImages.front(), &texture );
//        camImages.pop();
        mutex.release();
//    }
        loadTexture_Ipl(imagenText1, &texture );
	glutMainLoop ();
	
    //Free our texture
    FreeTexture( texture );
    
    
    return 0;
}
 */
