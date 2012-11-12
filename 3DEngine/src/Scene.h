#pragma once

#include "camera/PerspectiveCamera.h"
#include <vector>

#include "input/InputHandlerFactory.h"
#include "camera/InspectionCameraAdapter.h"
#include "camera/FirstPersonCameraAdapter.h"
#include "shape/Shape.h"
#include "camera/Camera.h"
#include "shader/ShaderBase.h"
#include "light/Light.h"

#include <vector>

using std::vector;

class Scene
{
public:
	Scene(InputHandlerFactory& ihf, Camera* cam);
	~Scene();

	void render();

	void addShape(Shape* shape);
	void addMaterial(ShaderBase* shape);
	void setCamera(Camera* shape);
	void addLight(Light* shape);

	static Scene* createDemoScene();

protected:

	InspectionCameraAdapter* mAdapter;
	FirstPersonCameraAdapter* mAdapter2;
	ShaderBase* shader;
	glm::mat4 lightTransform;

	Camera* activeCamera;

	vector<Shape*> objects;
	vector<ShaderBase*> materials;
	vector<Light*> lights;

};


