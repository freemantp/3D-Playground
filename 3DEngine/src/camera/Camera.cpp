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
	//Update view matrix
	viewMatrix = glm::lookAt( position, center, frame.up);
	
	//Update Camera frame
	frame.viewDir  = glm::normalize(center - position);
	frame.sideways = glm::normalize(glm::cross(frame.viewDir, frame.up ) );
}

const vec3& Camera::getPosition() const
{
	return position;
}

void Camera::setPosition(const vec3& pos, const vec3& up)
{
	position = pos;
	frame.up = up;
	updateViewMatrix();
}

const vec3& Camera::getCenter() const
{
	return center;
}

const CameraFrame& Camera::getFrame() const
{
	return frame;
}

float Camera::getNearPlane() const
{
	return nearP;
}

float Camera::getFarPlane() const
{
	return farP;
}
