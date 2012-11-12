#pragma once

#include <vector>

//fwd decls
class Camera;
class Shape;
class Light;
class ShaderBase;
class InspectionCameraAdapter;
class FirstPersonCameraAdapter;
class InputHandlerFactory;

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

	Camera* activeCamera;

protected:

	InspectionCameraAdapter* mAdapter;
	FirstPersonCameraAdapter* mAdapter2;
	ShaderBase* shader;
	glm::mat4 lightTransform;

	

	vector<Shape*> objects;
	vector<ShaderBase*> materials;
	vector<Light*> lights;

};


