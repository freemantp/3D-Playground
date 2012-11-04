#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;

class Camera
{
public:

	Camera(float nearPlane, float farPlane);

	virtual void init() = 0;

	mat4 getViewProjectionTransform() const;

	void updateViewMatrix();
	virtual void updateProjectionMatrix() = 0;

	const vec3& getPosition() const;
	const vec3& getCenter() const;
	const vec3& getUpVector() const;
	float getNearPlane() const;
	float getFarPlane() const;

	void setPosition(const vec3& pos);

	void setNearPlane(float near);
	void setFarPlane(float fov);

	mat4 viewMatrix;
	mat4 projectionMatrix;

protected:

	vec3 position;
	vec3 center;
	vec3 up;
	float nearP;
	float farP;
};

