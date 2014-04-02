#include "stdafx.h"
#include <glload/gl_load.hpp>
#include <ctime>
#include <string>

#include <rendering/Renderer.h>
#include <rendering/Viewport.h>

#include <scene/SceneParser.h>
#include <scene/Scene.h>
#include <util/Util.h>
#include <input/WindowEventHandler.h>
#include <config.h>

#include "GlutInputHandler.h"
#include "GlutInputHandlerFactory.H"
#include "TimeManager.h"


using std::string;

#define WINDOW_TITLE_PREFIX "OpenGL Playground"

Viewport_ptr viewport;

int	WindowHandle = 0;

bool Initialize();
bool InitializeGlut(int, char*[]);
void InitWindow();
void RenderFunction();

Renderer_ptr renderer;
//string sceneName = "headScene.xml";
//string sceneName = "manyPlanes.xml";
string sceneName = "shadowScene.xml";
//string sceneName = "road.xml";
//string sceneName = "simpleScene.xml";
//string sceneName = "shScene.xml";
//string sceneName = "ogreScene.xml";

int main(int argc, char* argv[])
{

	if (argc == 2)
	{
		sceneName = string(argv[1]);
	}

	int retCode = EXIT_SUCCESS;
	if (Initialize())
	{
		glutMainLoop();
	}
	else {
		glutHideWindow();
		getchar();
		retCode = EXIT_FAILURE;
	}

	exit(retCode);
}

void initGL()
{
	glload::LoadTest tst = glload::LoadFunctions();
	if (!tst)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
	}

	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);


	glEnable(GL_MULTISAMPLE);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	GLint  iMultiSample = 0;
	GLint  iNumSamples = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
	glGetIntegerv(GL_SAMPLES, &iNumSamples);

	std::cout << "MSAA: buffers= " << iMultiSample << ", samples = " << iNumSamples << std::endl;

}

bool Initialize()
{

	int glutArgc = 0;
	char** glutArgv = nullptr;

	viewport = Viewport::Create(1600, 900);

	if (!InitializeGlut(glutArgc, glutArgv))
		return false;

	InitWindow();
	initGL();

	fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

	GLint num;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);

	const char* data = Util::LoadTextFile(Config::SCENE_BASE_PATH + sceneName.c_str());


	renderer = Renderer::Create(viewport);

	GlutInputHandlerFactory gihf;
	SceneParser sp(gihf);
	if (sp.parse(data))
	{
		auto s = sp.GetScene();
		renderer->SetScene(s);
		TimeManager::GetInstance().AddTimeObserver(s);

		string windowTitle = string(WINDOW_TITLE_PREFIX) + " - " + s->name;
		glutSetWindowTitle(windowTitle.c_str());
	}
	else
	{
		Error("Scene could not be loaded: " + sceneName);
		return false;
	}

	return true;
}

bool InitializeGlut(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextFlags(GLUT_CORE_PROFILE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(viewport->width, viewport->height);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	return true;
}

void InitWindow()
{
	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (WindowHandle < 1)
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
	glutIdleFunc(TimeManager::Tick);

	//atexit(Controller::glutAtExit);

	glutReportErrors();

}

void RenderFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->Render();

	glutSwapBuffers();
	glutPostRedisplay();
}
