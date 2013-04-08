// Debug Playground.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include  <IL/il.h>
#include <iostream>
#include <vector>
#include <ctime>

using std::vector;

#include "util/Util.h"
#include "util/ObjLoader.h"
#include "camera/PerspectiveCamera.h"
#include "shader/PhongTextureShader.h"
#include "shape/Mesh.h"
#include "scene/Scene.h"
#include "input/InputHandlerFactory.h"
#include "util/MeshRaw.h"

void init(int argc, char* argv[])
{
	//GLUT
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
		);

	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//Window
	int	WindowHandle = glutCreateWindow("Debug Session");
	glutHideWindow();

	glutReportErrors();

	//Glew
	glewExperimental=GL_TRUE;
	GLenum err=glewInit();
	glGetError();
	if(err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout<<"glewInit failed, aborting." << std::endl;
	}

	ilInit();

	fprintf( stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION) );
}

Mesh_ptr loadModel()
{
	ObjLoader oj;
	clock_t begin = clock();
	
	MeshRaw* rawMesh = oj.loadObjFile("E:\\Development\\Graphics Playground\\data\\models\\rectangle.obj");

	if(rawMesh == NULL)
		return NULL;

	Mesh_ptr mesh = Mesh_ptr(new Mesh());

	mesh->setPositions(rawMesh->vertices,rawMesh->faces);

	if( rawMesh->hasTexCoords() )
		mesh->setTextureCoordinates(rawMesh->texCoords);
	
	if( rawMesh->hasNormals() )
		mesh->setNormals(rawMesh->normals);
	else	
		Warn("Normal data not present!");		

	if ( rawMesh->hasTangents() ) 
		mesh->setTangents(rawMesh->tangents);
	else
		Warn("Tangent data not present!");

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "time [msec]: " << elapsed_secs << std::endl;

	return mesh;
}

class MyInputHandler : public InputHandler
{
public:
	MyInputHandler() { };

	virtual void addMouseObserver(MouseObserver* observer) {};
	virtual void addKeyboardObserver(KeyboardObserver* observer) {};
};


class MyInputHandlerFactory : public InputHandlerFactory
{
public:
	virtual InputHandler& getInputHandler() 
	{
		return instance;	
	};
protected:
	static MyInputHandler instance;
};

MyInputHandler MyInputHandlerFactory::instance = MyInputHandler();

int main(int argc, char* argv[])
{

	init(argc, argv);

	PhongTextureShader shader("crate.jpg");

	MyInputHandlerFactory myF;
	Camera_ptr cam(new PerspectiveCamera(60));

	Scene s(myF,cam);

	Mesh_ptr m = loadModel();
	m->setShader(&shader);
	m->render(s);

	getchar();
	return 0;	

}

