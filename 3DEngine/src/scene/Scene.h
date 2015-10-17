#pragma once

#include <vector>
#include <array>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../util/SharedPointer.h"
#include "../math/BoundingBox.h"
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

	static Scene_ptr Create(const Camera_ptr& cam, bool has_frambufer = true);

	void Render(const Viewport_ptr& viewport);

	void AddShape(const Shape_ptr& shape);
	void SetSkybox(const Skybox_ptr& skybox);
	void AddMaterial(const ShaderBase* shape);
	void SetCamera(const Camera_ptr& cam);
	void AddLight(const PointLight_ptr& shape);
	void AddLight(const SpotLight_ptr& light);
	void SetLight(const DirectionalLight_ptr& light);
	void SetLight(const AmbientLight_ptr& light);

	AABBox BoundingBox() const;

	virtual void TimeUpdate(long time);

	Camera_ptr activeCamera;
	LightModel_ptr lightModel;
	std::vector<Shape_ptr> objects;
	std::vector<const ShaderBase*> materials;
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

	Scene(const Camera_ptr& cam,bool has_framebufer);

	virtual ~Scene();

	void RenderShadowMaps();

	InspectionCameraAdapter_ptr inspectionCamAdapter;
	FirstPersonCameraAdapter_ptr fpCamAdapter;

	ShadowMapShader_ptr shadowShader;
	Framebuffer_ptr framebuffer;

};


