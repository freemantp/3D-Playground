#include "stdafx.h"
#include <ctime>
#include "Scene.h"
#include "GlutInputHandler.h"

#include "Util.h"
#include "SceneParser.h"

#define WINDOW_TITLE_PREFIX "OpenGL Playground"

int CurrentWidth = 800,
	CurrentHeight = 800,
	WindowHandle = 0;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction();

int main(int argc, char* argv[])
{	
	const char* data = Util::loadTextFile(Config::SCENE_BASE_PATH  + "simpleScene.xml");

	SceneParser::parse(data);

	Initialize(argc, argv);
	glutMainLoop();

	getchar();
	exit(EXIT_SUCCESS);
}

GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */

bool light = false;
Scene* s;

void initGL()
{

	glewExperimental=GL_TRUE;
	GLenum err=glewInit();
	if(err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout<<"glewInit failed, aborting." << std::endl;
	}

	if(light) {
		/* Enable a single OpenGL light. */
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	}

	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);

}

void Initialize(int argc, char* argv[])
{
	InitWindow(argc, argv);
	initGL();

	fprintf( stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION) );

	GLint num;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);

	s = new Scene();
}

void InitWindow(int argc, char* argv[])
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

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if(WindowHandle < 1) 
	{

		Error("ERROR: Could not create a new rendering window");

		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);

	glutMouseFunc(GlutInputHandler::click);
	glutMotionFunc(GlutInputHandler::drag);
	glutPassiveMotionFunc(GlutInputHandler::mouseMove);
	glutMouseWheelFunc(GlutInputHandler::wheel);
	glutKeyboardFunc(GlutInputHandler::key);
	glutSpecialFunc(GlutInputHandler::specialKey);

	//atexit(Controller::glutAtExit);

	glutReportErrors();

}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);

	float aspectRatio = (float)Width / Height;

	s->resize(aspectRatio);
}



void RenderFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s->render();

	glutSwapBuffers();
	glutPostRedisplay();
}
