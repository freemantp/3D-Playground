#include "stdafx.h"
#include "Camera.h"

Camera::Camera(float nearPlane, float farPlane)
	: nearP(nearPlane)
	, farP(farPlane)
{

}

mat4 Camera::getViewProjectionTransform() const
{
	return projectionMatrix * viewMatrix;
}

void Camera::updateViewMatrix()
{
	viewMatrix = glm::lookAt( position, center, up);
}

const vec3& Camera::getPosition() const
{
	return position;
}

void Camera::setPosition(const vec3& pos)
{
	position = pos;
	updateViewMatrix();
}

const vec3& Camera::getCenter() const
{
	return center;
}

const vec3& Camera::getUpVector() const
{
	return up;
}

float Camera::getNearPlane() const
{
	return nearP;
}

float Camera::getFarPlane() const
{
	return farP;
}
