#include "stdafx.h"
#include <ctime>
#include <string>

#include "scene/Scene.h"
#include "GlutInputHandler.h"
#include "input/WindowEventHandler.h"
#include "GlutInputHandlerFactory.H"
#include "common.h"
#include "util/Util.h"
#include "scene/SceneParser.h"
#include "TimeManager.h"


#include  <IL/il.h>

using std::string;

#define WINDOW_TITLE_PREFIX "OpenGL Playground"

int CurrentWidth = 1600,
	CurrentHeight = 900,
	WindowHandle = 0;

bool Initialize();
bool InitializeGlut(int, char*[]);
void InitWindow();
void RenderFunction();

Scene_ptr s;
//string sceneName = "headScene.xml";
//string sceneName = "manyPlanes.xml";
string sceneName = "road.xml";
//string sceneName = "ogreScene.xml";

int main(int argc, char* argv[])
{	

	if(argc == 2)
	{
		sceneName = string(argv[1]);
	}

	
	int retCode = EXIT_SUCCESS;
	if( Initialize() )
	{
		glutMainLoop();
	} else {
		glutHideWindow();
		getchar();
		retCode = EXIT_FAILURE;
	}

	exit(retCode);
}

void initGL()
{
	glewExperimental=GL_TRUE;
	GLenum err=glewInit();
	glGetError(); //dummy call, see http://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW

	if(err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout<<"glewInit failed, aborting." << std::endl;
	}

	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);


	glEnable(GL_MULTISAMPLE);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	GLint  iMultiSample = 0;
	GLint  iNumSamples = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
	glGetIntegerv(GL_SAMPLES, &iNumSamples);

}

bool Initialize()
{

	int glutArgc = 0;
	char** glutArgv = nullptr;

	if( ! InitializeGlut(glutArgc, glutArgv) )
		return false;
	
	InitWindow();
	initGL();
	ilInit();

	fprintf( stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION) );

	GLint num;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);

	const char* data = Util::loadTextFile(Config::SCENE_BASE_PATH  + sceneName.c_str());
	
	GlutInputHandlerFactory gihf;
	SceneParser sp(gihf);	
	if(sp.parse(data))
	{
		s = sp.getScene();
		TimeManager::getInstance().addTimeObserver(s);

		string windowTitle = string(WINDOW_TITLE_PREFIX) + " - " + s->name;
		glutSetWindowTitle(windowTitle.c_str() );
	}
	else
	{
		Error("Scene could not be loaded: " + sceneName );	
		return false;
	}

	return true;


}

bool InitializeGlut(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	return true;
}

void InitWindow()
{
	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if(WindowHandle < 1) 
	{
		Error("ERROR: Could not create a new rendering window");
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(WindowEventHandler::resize);
	glutDisplayFunc(RenderFunction);

	glutMouseFunc(GlutInputHandler::click);
	glutMotionFunc(GlutInputHandler::drag);
	glutPassiveMotionFunc(GlutInputHandler::mouseMove);
	glutMouseWheelFunc(GlutInputHandler::wheel);
	glutKeyboardFunc(GlutInputHandler::key);
	glutSpecialFunc(GlutInputHandler::specialKey);
	glutIdleFunc(TimeManager::tick);

	//atexit(Controller::glutAtExit);

	glutReportErrors();

}

void RenderFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s->render();

	glutSwapBuffers();
	glutPostRedisplay();
}
