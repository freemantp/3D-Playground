#include "stdafx.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

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
	viewMatrix = glm::lookAt( position, target, frame.up);
	
	//Update Camera frame
	frame.viewDir  = glm::normalize(target - position);
	frame.sideways = glm::normalize(glm::cross(frame.viewDir, frame.up ) );
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

void Camera::setTarget(const vec3& pos)
{
	target = pos;
	updateViewMatrix();
}

void Camera::setUpVector(const vec3& up)
{
	frame.up = up;
	updateViewMatrix();
}


void Camera::setOrientation(const vec3& pos, const vec3& up)
{
	position = pos;
	frame.up = up;
	updateViewMatrix();
}

void Camera::setOrientation2(const vec3& target, const vec3& up)
{
	this->target = target;
	this->frame.up = up;

	updateViewMatrix();
}

const vec3& Camera::getTarget() const
{
	return target;
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