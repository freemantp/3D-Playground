///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// testing Frame Buffer Object (FBO) for "Render To Texture2D"
// OpenGL draws the scene directly to a texture object.
//
// GL_EXT_framebuffer_object extension is promoted to a core feature of OpenGL
// version 3.0 (GL_ARB_framebuffer_object)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-05-16
// UPDATED: 2013-07-08
///////////////////////////////////////////////////////////////////////////////

//#include <glload/gl_load.hpp>
#include <glload/gl_4_3_comp.h>
#include <glload/gl_load.hpp>
#include <GL/freeglut.h>	

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "glInfo.h"                             // glInfo struct
#include "Timer.h"
#include "teapot.h"

using std::stringstream;
using std::string;
using std::cout;
using std::endl;
using std::ends;


// GLUT CALLBACK functions ////////////////////////////////////////////////////
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

// function declearations /////////////////////////////////////////////////////
void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void toPerspective();
void draw();


// constants
const int   SCREEN_WIDTH = 400;
const int   SCREEN_HEIGHT = 300;
const float CAMERA_DISTANCE = 6.0f;
const int   TEXT_WIDTH = 8;
const int   TEXT_HEIGHT = 13;
const int   TEXTURE_WIDTH = 768;  // NOTE: texture size cannot be larger than
const int   TEXTURE_HEIGHT = 768;  // the rendering window size in non-FBO mode


// global variables
GLuint fboId;                       // ID of FBO
GLuint textureId;                   // ID of texture
GLuint rboId;                       // ID of Renderbuffer object
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode;
Timer timer, t1;
float playTime;                     // to compute rotation angle
float renderToTextureTime;          // elapsed time for render-to-texture


///////////////////////////////////////////////////////////////////////////////
// draw a textured cube with GL_TRIANGLES
///////////////////////////////////////////////////////////////////////////////
void draw()
{
	glBindTexture(GL_TEXTURE_2D, textureId);

	glColor4f(1, 1, 1, 1);
	glBegin(GL_TRIANGLES);
	// front faces
	glNormal3f(0, 0, 1);
	// face v0-v1-v2
	glTexCoord2f(1, 1);  glVertex3f(1, 1, 1);
	glTexCoord2f(0, 1);  glVertex3f(-1, 1, 1);
	glTexCoord2f(0, 0);  glVertex3f(-1, -1, 1);
	// face v2-v3-v0
	glTexCoord2f(0, 0);  glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0);  glVertex3f(1, -1, 1);
	glTexCoord2f(1, 1);  glVertex3f(1, 1, 1);

	// right faces
	glNormal3f(1, 0, 0);
	// face v0-v3-v4
	glTexCoord2f(0, 1);  glVertex3f(1, 1, 1);
	glTexCoord2f(0, 0);  glVertex3f(1, -1, 1);
	glTexCoord2f(1, 0);  glVertex3f(1, -1, -1);
	// face v4-v5-v0
	glTexCoord2f(1, 0);  glVertex3f(1, -1, -1);
	glTexCoord2f(1, 1);  glVertex3f(1, 1, -1);
	glTexCoord2f(0, 1);  glVertex3f(1, 1, 1);

	// top faces
	glNormal3f(0, 1, 0);
	// face v0-v5-v6
	glTexCoord2f(1, 0);  glVertex3f(1, 1, 1);
	glTexCoord2f(1, 1);  glVertex3f(1, 1, -1);
	glTexCoord2f(0, 1);  glVertex3f(-1, 1, -1);
	// face v6-v1-v0
	glTexCoord2f(0, 1);  glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 0);  glVertex3f(-1, 1, 1);
	glTexCoord2f(1, 0);  glVertex3f(1, 1, 1);

	// left faces
	glNormal3f(-1, 0, 0);
	// face  v1-v6-v7
	glTexCoord2f(1, 1);  glVertex3f(-1, 1, 1);
	glTexCoord2f(0, 1);  glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 0);  glVertex3f(-1, -1, -1);
	// face v7-v2-v1
	glTexCoord2f(0, 0);  glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 0);  glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 1);  glVertex3f(-1, 1, 1);

	// bottom faces
	glNormal3f(0, -1, 0);
	// face v7-v4-v3
	glTexCoord2f(0, 0);  glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 0);  glVertex3f(1, -1, -1);
	glTexCoord2f(1, 1);  glVertex3f(1, -1, 1);
	// face v3-v2-v7
	glTexCoord2f(1, 1);  glVertex3f(1, -1, 1);
	glTexCoord2f(0, 1);  glVertex3f(-1, -1, 1);
	glTexCoord2f(0, 0);  glVertex3f(-1, -1, -1);

	// back faces
	glNormal3f(0, 0, -1);
	// face v4-v7-v6
	glTexCoord2f(0, 0);  glVertex3f(1, -1, -1);
	glTexCoord2f(1, 0);  glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 1);  glVertex3f(-1, 1, -1);
	// face v6-v5-v4
	glTexCoord2f(1, 1);  glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 1);  glVertex3f(1, 1, -1);
	glTexCoord2f(0, 0);  glVertex3f(1, -1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}



///////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	// init global vars
	initSharedMem();

	// register exit callback
	atexit(exitCB);

	// init GLUT and GL
	initGLUT(argc, argv);
	initGL();

	// create a texture object
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object, you need to delete them when program exits.
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// create a renderbuffer object to store depth info
	// NOTE: A depth renderable image should be attached the FBO for depth test.
	// If we don't attach a depth renderable image to the FBO, then
	// the rendering output will be corrupted because of missing depth test.
	// If you also need stencil test for your rendering, then you must
	// attach additional image to the stencil attachement point, too.
	glGenRenderbuffers(1, &rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach a texture to FBO color attachement point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

	// attach a renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

	//@@ disable color buffer if you don't attach any color buffer image,
	//@@ for example, rendering the depth buffer only to a texture.
	//@@ Otherwise, glCheckFramebufferStatus will not be complete.
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	// start timer, the elapsed time will be used for rotating the teapot
	timer.start();

	// the last GLUT call (LOOP)
	// window will be shown and display callback is triggered by events
	// NOTE: this call never return main().
	glutMainLoop(); /* Start GLUT event-processing loop */

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
	// GLUT stuff for windowing
	// initialization openGL window.
	// It must be called before any other GLUT routine.
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);


	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);   // display mode

	glutInitWindowSize(screenWidth, screenHeight);              // window size

	glutInitWindowPosition(100, 100);                           // window location

	// finally, create a window with openGL context
	// Window will not displayed until glutMainLoop() is called
	// It returns a unique ID.
	int handle = glutCreateWindow(argv[0]);     // param is the title of window

	// register GLUT callback functions
	glutDisplayFunc(displayCB);
	//glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
	glutIdleFunc(idleCB);                       // redraw whenever system is idle
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);

	return handle;
}



///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
	glload::LoadTest tst = glload::LoadFunctions();
	if (!tst)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);


	// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0);                   // background color
	glClearDepth(1.0f);                         // 0 is near, 1 is far
	//glDepthFunc(GL_LEQUAL);

	initLights();
}


///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;

	mouseLeftDown = mouseRightDown = false;
	mouseX = mouseY = 0;

	cameraAngleX = cameraAngleY = 45;
	cameraDistance = CAMERA_DISTANCE;

	drawMode = 0; // 0:fill, 1: wireframe, 2:points

	fboId = rboId = textureId = 0;
	playTime = renderToTextureTime = 0;

	return true;
}



///////////////////////////////////////////////////////////////////////////////
// clean up global variables
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
	glDeleteTextures(1, &textureId);
	textureId = 0;

	glDeleteFramebuffers(1, &fboId);
	fboId = 0;
	glDeleteRenderbuffers(1, &rboId);
	rboId = 0;
	
}



///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { .2f, .2f, .2f, 1.0f };  // ambient light
	GLfloat lightKd[] = { .7f, .7f, .7f, 1.0f };  // diffuse light
	GLfloat lightKs[] = { 1, 1, 1, 1 };           // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = { 0, 0, 20, 1 }; // positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}



///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
	// set viewport to be the entire window
	glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

	// set perspective viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(screenWidth) / screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

	// switch to modelview matrix in order to set scene
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
	// get the total elapsed time
	playTime = (float)timer.getElapsedTime();

	// compute rotation angle
	const float ANGLE_SPEED = 90;   // degree/s
	float angle = ANGLE_SPEED * playTime;


	// adjust viewport and projection matrix to texture dimension
	glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(TEXTURE_WIDTH) / TEXTURE_HEIGHT, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	// camera transform
	glLoadIdentity();
	glTranslatef(0, 0, -CAMERA_DISTANCE);

	// with FBO
	// render directly to a texture

	// set the rendering destination to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// clear buffer
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw a rotating teapot at the origin
	glPushMatrix();
	glRotatef(angle*0.5f, 1, 0, 0);
	glRotatef(angle, 0, 1, 0);
	glRotatef(angle*0.7f, 0, 0, 1);
	glTranslatef(0, -1.575f, 0);
	drawTeapot();
	glPopMatrix();

	// back to normal window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

	// trigger mipmaps generation explicitly
	// NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
	// triggers mipmap generation automatically. However, the texture attached
	// onto a FBO should generate mipmaps manually via glGenerateMipmap().
	glBindTexture(GL_TEXTURE_2D, textureId);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// rendering as normal ////////////////////////////////////////////////////

	// back to normal viewport and projection matrix
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)(screenWidth) / screenHeight, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// tramsform camera
	glTranslatef(0, 0, -cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);   // pitch
	glRotatef(cameraAngleY, 0, 1, 0);   // heading

	// clear framebuffer
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();

	// draw a cube with the dynamic texture
	draw();

	glPopMatrix();

	glutSwapBuffers();
}


void reshapeCB(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	toPerspective();
}


void timerCB(int millisec)
{
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}


void idleCB()
{
	glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // ESCAPE
		exit(0);
		break;
	default:
		;
	}
}


void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}


void mouseMotionCB(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistance -= (y - mouseY) * 0.2f;
		mouseY = y;
	}
}


void exitCB()
{
	clearSharedMem();
}
