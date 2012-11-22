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

Mesh* loadModel()
{
	ObjLoader oj;
	clock_t begin = clock();
	
	if(!oj.loadObjFile("E:\\Development\\Graphics Playground\\data\\models\\rectangle.obj"))
		return NULL;

	Mesh* mesh = new Mesh();
	vector<float> vertexArray;
	vector<int> indexArray;

	oj.getVertexArray(vertexArray);
	oj.getIndexArray(indexArray);
	mesh->setPositions(vertexArray,indexArray);

	if( oj.hasTexCoords() )
	{
		vector<float> texCoordArray;
		oj.getTexCoordArray(texCoordArray);
		mesh->setTextureCoordinates(texCoordArray);
	}


	if( ! oj.hasNormals() )
	{		
		Warn("Normal data not present... computing normals");		
		if(! oj.computeNormals() )
			Error("Could not compute normals");
	}	

	if ( oj.computeTangents() ) 
	{		
		vector<float> tangentArray;
		oj.getTangentArray(tangentArray);
		mesh->setTangents(tangentArray);
	}
	else
	{
		Error("Could not compute tangents");
	}
		
	vector<float> normalArray;
	oj.getNormalArray(normalArray);
	mesh->setNormals(normalArray);

	//mesh->setColors(vertexArray);

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
	Scene s(myF,new PerspectiveCamera(60));

	Mesh* m = loadModel();
	m->setShader(&shader);

	m->render(s);

	if(m != NULL)
		delete m;

	getchar();
	return 0;	

}

