#pragma once

#include "Graphics Playground.h"

#pragma warning(disable: 4251) // See https://github.com/cginternals/glbinding/issues/141
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/functions.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>
#include <glbinding-aux/debug.h>
#pragma warning(default: 4251)

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cstdlib> 

#include <input/WindowEventHandler.h>
#include <shader/ShaderLibrary.h>
#include <scene/SceneParser.h>
#include <scene/Scene.h>
#include <util/TimeManager.h>
#include <util/Util.h>
#include <config.h>
#include <error.h>

#include "GlfwInputHandler.h"

using std::string;
using namespace gl;
using namespace glbinding;

int main(int argc, char* argv[])
{

	std::vector<std::string> scenes = {
		"simpleScene.xml", //0
		"boxScene.xml",    //1
		"envHorse.xml",    //2
		"headScene.xml",   //3
		"manyPlanes.xml",  //4
		"shadowScene.xml", //5
		"road.xml",        //6
		"simpleScene.xml", //7
		"shScene.xml",     //8
		"ogreScene.xml",   //9
		"debug.xml",       //10
		"blackhawk.xml",   //11
		"headScene.xml"    //12
	};

	std::string sceneName = scenes[1];

	if (argc == 2)
		sceneName = string(argv[1]);

	atexit(glfwTerminate);

	//Scoped shared pointers -> force cleanup of OpenGL resources
	{
		GraphicsPlayground pg;
		if (pg.InitializeWindow())
		{
			if (pg.LoadScene(sceneName))
			{
				pg.Render();
			}
			else
			{
				Error("Scene could not be loaded: " + sceneName);
				return -1;
			}
		}
		else
		{
			Error("Window initialization failed, exiting");
			return -1;
		}
	}

	return 0;
}

GraphicsPlayground::GraphicsPlayground()
	: window(nullptr)
	, numMultisamples(4)
{
	viewport = Viewport::Create(1600, 900);
	renderer = Renderer::Create(viewport);
}

GraphicsPlayground::~GraphicsPlayground()
{
	ShaderLibrary::Instance()->Reset();
}

void GraphicsPlayground::InitGL()
{
	glbinding::initialize(glfwGetProcAddress);
	glbinding::aux::enableGetErrorCallback();

	// Use depth buffering for hidden surface elimination
	glEnable(GL_DEPTH_TEST);

	GLint iMultiSample = 0;
	GLint iNumSamples = 0;
	glEnable(GL_MULTISAMPLE);
	glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
	glGetIntegerv(GL_SAMPLES, &iNumSamples);

	std::cout << "MSAA: buffers= " << iMultiSample << ", samples = " << iNumSamples << std::endl;

		std::cout << std::endl
		<< "OpenGL Version:  " << aux::ContextInfo::version() << std::endl
		<< "OpenGL Vendor:   " << aux::ContextInfo::vendor() << std::endl
		<< "OpenGL Renderer: " << aux::ContextInfo::renderer() << std::endl;
}

bool GraphicsPlayground::InitializeWindow()
{	
	glfwSetErrorCallback([](int errnum, const char* errmsg) {
		std::cerr << errnum << ": " << errmsg << std::endl; });
	
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_SAMPLES, numMultisamples);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(viewport->width, viewport->height, WINDOW_TITLE_PREFIX.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	InitGL();

	return window;
}

void GraphicsPlayground::Render() {

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		TimeManager::Tick(glfwGetTime());
	}
}

bool GraphicsPlayground::LoadScene(std::string& sceneName) {

	auto sceneData = Util::LoadTextFile(Config::SCENE_BASE_PATH + sceneName.c_str());

	if (sceneData.length() > 0) {
		SceneParser sceneParser;
		if (sceneParser.Parse(sceneData))
		{
			auto scene = sceneParser.Scene();
			TimeManager::Instance().AddTimeObserver(scene);

			SetWindowTitle(scene->name);
			InitializeEvents(scene);
			renderer->SetScene(scene);
			WindowEventHandler::ViewportChanged(viewport);
			return true;
		}
	}

	return false;
}

void GraphicsPlayground::InitializeEvents(Scene_ptr& scene) {

	WindowEventHandler& winEventHandler = WindowEventHandler::Instance();
	winEventHandler.AddViewportObserver(renderer);

	GlfwInputHandler& inputHandler = GlfwInputHandler::Instance();
	inputHandler.AddKeyboardObserver(renderer);

	scene->ConnectInputHandler(inputHandler);

	glfwSetKeyCallback(window, GlfwInputHandler::key);
	glfwSetMouseButtonCallback(window, GlfwInputHandler::click);
	glfwSetCursorPosCallback(window, GlfwInputHandler::mouseMove);
	glfwSetScrollCallback(window, GlfwInputHandler::wheel);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		WindowEventHandler::Resize(width, height);
	});

}

const std::string GraphicsPlayground::WINDOW_TITLE_PREFIX = "OpenGL Playground";

void GraphicsPlayground::SetWindowTitle(const std::string title)
{
	string windowTitle = string(WINDOW_TITLE_PREFIX) + " - " + title;
	glfwSetWindowTitle(window, windowTitle.c_str());
}
