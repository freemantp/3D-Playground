#pragma once

#include <vector>
#include <memory>
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
class Skybox;

class Scene;

class Scene : public TimeObserver
{
public:		

	typedef std::shared_ptr<Scene> Ptr;

	Scene(InputHandlerFactory& ihf, Camera* cam);
	~Scene();

	void render();

	void addShape(Shape* shape);
	void setSkybox(Skybox* skybox);
	void addMaterial(ShaderBase* shape);
	void setCamera(Camera* shape);
	void addLight(PointLight* shape);
	void addLight(SpotLight* light);

	virtual void timeUpdate(long time);

	Camera* activeCamera;
	LightModel lightModel;
	std::vector<Shape*> objects;
	std::vector<ShaderBase*> materials;
	Skybox* skybox;
	std::string name;

protected:

	InspectionCameraAdapter* mAdapter;
	FirstPersonCameraAdapter* mAdapter2;
	

};


