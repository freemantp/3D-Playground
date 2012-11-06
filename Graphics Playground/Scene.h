#pragma once

#include "TestCamera.h"
#include <vector>
#include "Shape.h"

class Scene
{
public:
	Scene();
	~Scene();

	void render();
	void resize(float aspectRatio);

protected:

	ShaderBase* getShader(const string& shaderName);
	void initContent();

	MouseCameraAdapter* mAdapter;
	ShaderBase* shader;
	glm::mat4 lightTransform;
	std::vector<Shape*> objects;
	TestCamera* cam;
	//glm::vec4 initialLightPos;
};

