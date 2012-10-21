#include "stdafx.h"

#include "Box.h"
#include "Triangle.h"
#include "TestCamera.h"
#include "GLSLProgram.h"
#include <glm/gtc/matrix_transform.hpp>

#define WINDOW_TITLE_PREFIX "Box"

int CurrentWidth = 800,
	CurrentHeight = 800,
	WindowHandle = 0;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);


int main(int argc, char* argv[])
{
	Initialize(argc, argv);


	glutMainLoop();

	getchar();

	exit(EXIT_SUCCESS);
}

GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */

Triangle* tri;
Box* box;
Camera* cam;

bool light = false;

void initGL(void)
{

	glewExperimental=GL_TRUE;
	GLenum err=glewInit();
	if(err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		cout<<"glewInit failed, aborting."<<endl;
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

	cam = new TestCamera();
	cam->init();

	tri = new Triangle();
	box = new Box();
	tri->init();
	box->init();

}

GLSLProgram* p;

void Initialize(int argc, char* argv[])
{

	InitWindow(argc, argv);
	initGL();

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
		);

	string vertexShaderSource = GLSLProgram::loadSource("../data/shader/basic.vert");
	string fragmentShaderSource = GLSLProgram::loadSource("../data/shader/basic.frag");
	
	p = new GLSLProgram();

	bool success = p->compileShaderFromString(vertexShaderSource, GLSLShader::VERTEX);
	success = p->compileShaderFromString(fragmentShaderSource, GLSLShader::FRAGMENT);
	success = p->link();

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

	if(WindowHandle < 1) {

		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
			);
		exit(EXIT_FAILURE);
	}

	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);

}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}



void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//cam->apply();

	/*glm::mat4 ident = glm::mat4(1.0f);
	glm::mat4 world = glm::translate(ident, glm::vec3(0.0f,0.0f,-1.0f));
	world = glm::rotate(world,60.0f,glm::vec3(1.0f,0.0f,0.0f));
	world = glm::rotate(world,20.0f,glm::vec3(0.0f,0.0f,1.0f));

	glm::mat4 mvp = cam->getViewProjectionTransform() * world;*/

	glBindAttribLocation(p->getProgramHandle(), 0, "VertexPosition");
	glBindAttribLocation(p->getProgramHandle(), 1, "VertexColor");

	p->use();
	tri->render(cam);
	p->unuse();

	glutSwapBuffers();
	glutPostRedisplay();
}
