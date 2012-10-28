#pragma once

#include "TestCamera.h"
#include <vector>
#include "Shape.h"

class Scene
{
public:
	Scene(void);
	~Scene(void);

	void render(void);
	void resize(float aspectRatio);

protected:

	ShaderBase* getShader(const string& shaderName);
	void initContent(void);

	ShaderBase* shader;
	std::vector<Shape*> objects;
	TestCamera* cam;
};

