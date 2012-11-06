#pragma once

#include "camera/PerspectiveCamera.h"
#include <vector>

#include "camera/InspectionCameraAdapter.h"
#include "camera/FirstPersonCameraAdapter.h"
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

	InspectionCameraAdapter* mAdapter;
	FirstPersonCameraAdapter* mAdapter2;
	ShaderBase* shader;
	glm::mat4 lightTransform;
	std::vector<Shape*> objects;
	PerspectiveCamera* cam;

};

