#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../input/ViewportObserver.h"

using glm::mat4;
using glm::vec3;

struct CameraFrame
{
	vec3 up;
	vec3 viewDir;
	vec3 sideways;
};

class Camera : public ViewportObserver
{
public:

	Camera(float nearPlane, float farPlane);

	mat4 getViewProjectionTransform() const;

	void updateViewMatrix();
	virtual void updateProjectionMatrix() = 0;
	virtual void viewportSizeChanged(int width, int height) = 0;

	const vec3& getPosition() const;
	const vec3& getTarget() const;
	const CameraFrame& getFrame() const;
	float getNearPlane() const;
	float getFarPlane() const;

	void setPosition(const vec3& pos);
	void setTarget(const vec3& pos);
	void setUpVector(const vec3& pos);
	void setOrientation(const vec3& pos, const vec3& up);
	void setOrientation2(const vec3& target, const vec3& up);

	void setNearPlane(float near);
	void setFarPlane(float fov);

	mat4 viewMatrix;
	mat4 projectionMatrix;

protected:

	vec3 position;
	vec3 target;
	CameraFrame frame;
	float nearP;
	float farP;
};

