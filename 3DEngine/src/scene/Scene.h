#pragma once

#include <vector>
#include <array>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../util/SharedPointer.h"
#include "../light/LightModel.h"
#include "../animation/TimeObserver.h"

//fwd decls
class ShaderBase;
class InputHandler;

SHARED_PTR_CLASS_DECL(Camera);	
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(Skybox);
SHARED_PTR_CLASS_DECL(InspectionCameraAdapter);
SHARED_PTR_CLASS_DECL(FirstPersonCameraAdapter);
SHARED_PTR_CLASS_DECL(ShadowMapShader);
SHARED_PTR_CLASS_DECL(Framebuffer);
SHARED_PTR_CLASS_DECL(Viewport);
SHARED_PTR_CLASS_DECL(AmbientLight)

class Scene : public TimeObserver, public std::enable_shared_from_this<Scene>
{
public:		

	static Scene_ptr Create(Camera_ptr cam, bool has_frambufer = true);

	void Render(Viewport_ptr viewport);

	void AddShape(Shape_ptr shape);
	void SetSkybox(Skybox_ptr skybox);
	void AddMaterial(ShaderBase* shape);
	void SetCamera(Camera_ptr cam);
	void AddLight(PointLight_ptr shape);
	void AddLight(SpotLight_ptr light);
	void SetLight(DirectionalLight_ptr light);
	void SetLight(AmbientLight_ptr light);

	virtual void TimeUpdate(long time);

	Camera_ptr activeCamera;
	LightModel_ptr lightModel;
	std::vector<Shape_ptr> objects;
	std::vector<ShaderBase*> materials;
	Skybox_ptr skybox;
	std::string name;
	
	class LightAnimationParam
	{
		public:
			glm::vec3 rotationAxis;
			float radiansPerInterval;
	};

	void ConnectInputHandler(InputHandler& ih);

	std::array<LightAnimationParam,3> lightAnimParams;

protected:

	Scene(Camera_ptr cam,bool has_framebufer);

	virtual ~Scene();

	void RenderShadowMaps();

	InspectionCameraAdapter_ptr inspectionCamAdapter;
	FirstPersonCameraAdapter_ptr fpCamAdapter;

	ShadowMapShader_ptr shadowShader;
	Framebuffer_ptr framebuffer;

};


