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
	
	ObjLoader oj;

	clock_t begin = clock();
	oj.loadObjFile("../data/models/cube.obj");
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "time [msec]: " << elapsed_secs << std::endl;

	/*Initialize(argc, argv);


	glutMainLoop();*/

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

	/*char* path = "../data/models/cube.obj";

	//Load Alias Wavefront obj file	
	objData = new objLoader();

	cout << "Loading "<< path << " ";
	if(objData->load(path) == 1) {

		//Check for presence of vertex normals
		if(objData->vertexCount == objData->normalCount) {
			cout << "succeeded " << "(" << objData->vertexCount << " vertices/normals, " << objData->faceCount << " faces)" << endl;
		}
		else {
			cerr << "failed: missing vertex normals" << endl;
		}
	} else {
		cerr << "failed: Obj file could not be loaded" << endl;
	}

	vector<float> positions;
	vector<float> normals;
	vector<int> indices;

	positions.reserve(objData->vertexCount * 3);
	normals.reserve(objData->normalCount * 3);
	indices.reserve(objData->faceCount * 3);

	//Iterate over faces
	for(int i=0; i < objData->vertexCount; i++) 
	{            
		obj_vector* v = objData->vertexList[i];
		positions.push_back((float)v->e[0]);
		positions.push_back((float)v->e[1]);
		positions.push_back((float)v->e[2]);
		
		obj_vector* n = objData->normalList[i];	
		normals.push_back((float)n->e[0]);
		normals.push_back((float)n->e[1]);
		normals.push_back((float)n->e[2]);
	}

	for(int i=0; i < objData->faceCount; i++) 
	{
		obj_face *f  = objData->faceList[i];
		indices.push_back(f->vertex_index[0]);
		indices.push_back(f->vertex_index[1]);
		indices.push_back(f->vertex_index[2]);
	}
	
	cow = new Mesh();
	cow->setPositions(positions,indices);
	cow->setNormals(normals);*/

	tri = new Triangle();
	box = new Box();

	tri->init();
	box->init();

	if(shaderSet) {
		tri->setShader(p);
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

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tri->render(*(const Camera*)cam);

	glutSwapBuffers();
	glutPostRedisplay();
}
