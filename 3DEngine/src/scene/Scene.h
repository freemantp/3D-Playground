#pragma once

#include <vector>
#include "../util/SharedPointer.h"
#include "../light/LightModel.h"
#include "../animation/TimeObserver.h"

//fwd decls
class ShaderBase;
class InputHandlerFactory;
class Skybox;

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(Skybox);
SHARED_PTR_CLASS_DECL(InspectionCameraAdapter);
SHARED_PTR_CLASS_DECL(FirstPersonCameraAdapter);

class Scene : public TimeObserver
{
public:		

	Scene(InputHandlerFactory& ihf, Camera_ptr cam);
	~Scene();

	void render();

	void addShape(Shape_ptr shape);
	void setSkybox(Skybox_ptr skybox);
	void addMaterial(ShaderBase* shape);
	void setCamera(Camera_ptr cam);
	void addLight(PointLight_ptr shape);
	void addLight(SpotLight_ptr light);

	virtual void timeUpdate(long time);

	Camera_ptr activeCamera;
	LightModel_ptr lightModel;
	std::vector<Shape_ptr> objects;
	std::vector<ShaderBase*> materials;
	Skybox_ptr skybox;
	std::string name;

protected:

	InspectionCameraAdapter_ptr mAdapter;
	FirstPersonCameraAdapter_ptr mAdapter2;
	

};


