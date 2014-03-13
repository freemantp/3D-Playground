#pragma once

#include <vector>
#include "../util/SharedPointer.h"
#include "../light/LightModel.h"
#include "../animation/TimeObserver.h"

//fwd decls
class ShaderBase;
class InputHandlerFactory;

SHARED_PTR_CLASS_DECL(Camera);	
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(Skybox);
SHARED_PTR_CLASS_DECL(InspectionCameraAdapter);
SHARED_PTR_CLASS_DECL(FirstPersonCameraAdapter);
SHARED_PTR_CLASS_DECL(ShadowMapShader);
SHARED_PTR_CLASS_DECL(Framebuffer);
SHARED_PTR_CLASS_DECL(Viewport);

class Scene 
	: public TimeObserver
	, public std::enable_shared_from_this<Scene>
{
public:		

	static Scene_ptr Create(InputHandlerFactory& ihf, Camera_ptr cam);

	virtual ~Scene();

	void render(Viewport_ptr viewport);

	void AddShape(Shape_ptr shape);
	void SetSkybox(Skybox_ptr skybox);
	void AddMaterial(ShaderBase* shape);
	void SetCamera(Camera_ptr cam);
	void AddLight(PointLight_ptr shape);
	void AddLight(SpotLight_ptr light);

	virtual void TimeUpdate(long time);

	Camera_ptr activeCamera;
	LightModel_ptr lightModel;
	std::vector<Shape_ptr> objects;
	std::vector<ShaderBase*> materials;
	Skybox_ptr skybox;
	std::string name;

protected:

	Scene(InputHandlerFactory& ihf, Camera_ptr cam);

	InspectionCameraAdapter_ptr mAdapter;
	FirstPersonCameraAdapter_ptr mAdapter2;

	ShadowMapShader_ptr shadowShader;
	Framebuffer_ptr framebuffer;

};


