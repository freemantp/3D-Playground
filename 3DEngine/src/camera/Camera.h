#pragma once

#include <glm/glm.hpp>
#include "../input/ViewportObserver.h"
#include "../util/SharedPointer.h"

using glm::mat4;
using glm::vec3;

struct CameraFrame
{
	vec3 up;
	vec3 viewDir;
	vec3 sideways;
};

SHARED_PTR_CLASS_DECL(Camera);

class Camera : public ViewportObserver
{
public:

	Camera(float nearPlane, float farPlane);

	mat4 GetViewProjectionTransform() const;

	void UpdateViewMatrix();
	virtual void UpdateProjectionMatrix() = 0;

	const vec3& GetPosition() const;
	const vec3& GetTarget() const;
	const CameraFrame& GetFrame() const;
	float GetNearPlane() const;
	float GetFarPlane() const;

	void SetPosition(const vec3& pos);
	void SetTarget(const vec3& pos);
	void SetUpVector(const vec3& pos);
	void SetOrientation(const vec3& pos, const vec3& up);
	void SetOrientation2(const vec3& target, const vec3& up);

	void SetNearPlane(float near);
	void SetFarPlane(float fov);

	mat4 viewMatrix;
	mat4 projectionMatrix;

protected:

	vec3 position;
	vec3 target;
	CameraFrame frame;
	float nearP;
	float farP;
};

