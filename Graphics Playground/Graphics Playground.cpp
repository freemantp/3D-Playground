#include "stdafx.h"

#include "Box.h"
#include "Triangle.h"
#include "TestCamera.h"
#include "GLSLProgram.h"
#include "Util.h"

#include <objLoader.h>
#include <ctime>


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
Mesh* cow;
Camera* cam;
GLSLProgram* p;
//objLoader* objData;

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

}

bool getShader()
{
	p = new GLSLProgram();

	string vertexShaderSource = Util::loadTextFile("../data/shader/basic.vert");
	string fragmentShaderSource = Util::loadTextFile("../data/shader/basic.frag");

	if (!p->compileShaderFromString(vertexShaderSource, GLSLShader::VERTEX))  
	{
		Error("--- Vertex Shader ---\n" + p->log());
		return false;
	}

	if (!p->compileShaderFromString(fragmentShaderSource, GLSLShader::FRAGMENT)) 
	{
		Error("--- Fragment Shader ---\n" + p->log());
		return false;
	}

	if (!p->link()) {
		Error("--- Linker ---\n" + p->log());
		return false;
	}

	int pos =  glGetAttribLocation(p->getProgramHandle(),"vertexPosition");
	int norm = glGetAttribLocation(p->getProgramHandle(),"vertexNormal");
	int col =  glGetAttribLocation(p->getProgramHandle(),"vertexColor");

	Util::printStrings(p->getVertexAttributes());
	Util::printStrings(p->getUniformAttributes());
	return true;
}

void initContent(void)
{
	cam = new TestCamera();
	cam->init();

	bool shaderSet = getShader();


	ObjLoader oj;

	clock_t begin = clock();
	oj.loadObjFile("../data/models/cube.obj");
	oj.computeNormals();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "time [msec]: " << elapsed_secs << std::endl;

	vector<float> vertexArray, normalArray;
	vector<int> indexArray;

	oj.getVertexArray(vertexArray);
	oj.getIndexArray(indexArray);
	oj.getNormalArray(normalArray);

	cow = new Mesh();
	cow->setPositions(vertexArray,indexArray);
	cow->setNormals(normalArray);

	tri = new Triangle();
	box = new Box();

	tri->init();
	box->init();

	if(shaderSet) {
		tri->setShader(p);
		box->setShader(p);
		cow->setShader(p);
	}
	else
		Error("Shader was not loaded");
}

void Initialize(int argc, char* argv[])
{

	InitWindow(argc, argv);
	initGL();
	

	fprintf( stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION) );

	GLint num;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num);

	initContent();

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

}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

#include <glm/gtc/matrix_transform.hpp>

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shape* s = cow;

	s->render(*(const Camera*)cam);
	s->worldTransform = glm::rotate(s->worldTransform,1.0f,glm::vec3(1.0f,1.0f,1.0f));   

	glutSwapBuffers();
	glutPostRedisplay();
}
