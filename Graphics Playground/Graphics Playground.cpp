#pragma once

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

#include <rendering/Renderer.h>
#include <rendering/Viewport.h>
#include <input/WindowEventHandler.h>
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

#define WINDOW_TITLE_PREFIX "OpenGL Playground"

string sceneName = "simpleScene.xml";
//string sceneName = "boxScene.xml";
//string sceneName = "envHorse.xml";
//string sceneName = "headScene.xml";
//string sceneName = "manyPlanes.xml";
//string sceneName = "shadowScene.xml";
//string sceneName = "road.xml";
//string sceneName = "simpleScene.xml";
//string sceneName = "shScene.xml";
//string sceneName = "ogreScene.xml";
//string sceneName = "debug.xml";
//string sceneName = "pc24.xml";

void initGL()
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
}

void error(int errnum, const char* errmsg)
{
	std::cerr << errnum << ": " << errmsg << std::endl;
}

GLFWwindow* InitializeWindow(Viewport_ptr& viewport, std::string& sceneName) 
{	
	glfwSetErrorCallback(error);
	
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_SAMPLES, 4);

	string windowTitle = string(WINDOW_TITLE_PREFIX) + " - " + sceneName;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(viewport->width, viewport->height, windowTitle.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initGL();

	std::cout << std::endl
		<< "OpenGL Version:  " << aux::ContextInfo::version() << std::endl
		<< "OpenGL Vendor:   " << aux::ContextInfo::vendor() << std::endl
		<< "OpenGL Renderer: " << aux::ContextInfo::renderer() << std::endl;

	return window;
}

void Render(GLFWwindow* window, Renderer_ptr renderer) {

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

Scene_ptr LoadScene(std::string& sceneName) {

	auto sceneData = Util::LoadTextFile(Config::SCENE_BASE_PATH + sceneName.c_str());

	SceneParser sceneParser;
	if (sceneParser.Parse(sceneData))
	{
		auto scene = sceneParser.Scene();
		TimeManager::Instance().AddTimeObserver(scene);
		return scene;
	}
	else
	{
		Error("Scene could not be loaded: " + sceneName);
		return Scene_ptr();
	}
}

void InitializeEvents(GLFWwindow* window, Renderer_ptr& renderer, Scene_ptr& scene) {

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

int main(int argc, char* argv[])
{
	if (argc == 2)
		sceneName = string(argv[1]);

	Viewport_ptr viewport = Viewport::Create(1600, 900);
	Renderer_ptr renderer = Renderer::Create(viewport);
	GLFWwindow* window = InitializeWindow(viewport, sceneName);

	Scene_ptr scene = LoadScene(sceneName);
	if(scene)
	{ 
		renderer->SetScene(scene);

		if (window == nullptr) {
			std::cerr << "Window initialization failed, exiting" << std::endl;
			return -1;
		}

		InitializeEvents(window, renderer, scene);

		WindowEventHandler::ViewportChanged(viewport);

		Render(window, renderer);
	}


	glfwTerminate();
	return 0;
}

