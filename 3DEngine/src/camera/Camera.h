#pragma once

#include "../input/ViewportObserver.h"
#include "../util/SharedPointer.h"

#include <glm/glm.hpp>

struct CameraFrame
{
	glm::vec3 up;
	glm::vec3 viewDir;
	glm::vec3 sideways;
};

class Camera : public ViewportObserver
{
public:

	Camera(float nearPlane, float farPlane);

	glm::mat4 ViewProjectionTransform() const;

	glm::mat4 ProjectionTransform() const;

	void UpdateViewMatrix();
	virtual void UpdateProjectionMatrix() = 0;

	const glm::vec3& Position() const;
	const glm::vec3& Target() const;
	const CameraFrame& Frame() const;
	float NearPlane() const;
	float FarPlane() const;

	void SetPosition(const glm::vec3& pos);
	void SetTarget(const glm::vec3& target);
	void SetUpVector(const glm::vec3& up);
	void SetOrientation(const glm::vec3& pos, const glm::vec3& up);
	void SetOrientation2(const glm::vec3& target, const glm::vec3& up);

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

protected:

	glm::vec3 position;
	glm::vec3 target;
	CameraFrame frame;
	float nearP;
	float farP;
};

