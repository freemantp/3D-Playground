#pragma once

#include "../input/ViewportObserver.h"
#include "../util/SharedPointer.h"

#include <glm/glm.hpp>

struct CameraFrame : public glm::mat3
{
	glm::vec3& ViewDir() { return operator[](0); };
	glm::vec3& Side() { return operator[](1); };
	glm::vec3& Up() { return operator[](2); };
	
	const glm::vec3& ViewDir() const { return operator[](0); };
	const glm::vec3& Side() const { return operator[](1); };
	const glm::vec3& Up() const { return operator[](2); };

	CameraFrame()
	{
		ViewDir() = glm::vec3(1, 0, 0);
		Side() = glm::vec3(0, 0, 1);
		Up() = glm::vec3(0, 1, 0);
	};
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
	CameraFrame& Frame();
	float NearPlane() const;
	float FarPlane() const;

	void SetPosition(const glm::vec3& pos);

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

protected:

	glm::vec3 position;
	CameraFrame frame;
	float nearP;
	float farP;
};

