#pragma once

#include <vector>
#include "../light/LightModel.h"
#include "../animation/TimeObserver.h"

//fwd decls
class Camera;
class Shape;
class PointLight;
class ShaderBase;
class InspectionCameraAdapter;
class FirstPersonCameraAdapter;
class InputHandlerFactory;
class LightModel;

using std::vector;

class Scene : public TimeObserver
{
public:
	Scene(InputHandlerFactory& ihf, Camera* cam);
	~Scene();

	void render();

	void addShape(Shape* shape);
	void addMaterial(ShaderBase* shape);
	void setCamera(Camera* shape);
	void addLight(PointLight* shape);
	void addLight(SpotLight* light);

	virtual void timeUpdate(long time);

	Camera* activeCamera;
	LightModel lightModel;
	vector<Shape*> objects;
	vector<ShaderBase*> materials;

protected:

	InspectionCameraAdapter* mAdapter;
	FirstPersonCameraAdapter* mAdapter2;

};


