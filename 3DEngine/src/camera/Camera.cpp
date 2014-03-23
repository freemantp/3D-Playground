#include "stdafx.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float nearPlane, float farPlane)
	: nearP(nearPlane)
	, farP(farPlane)
{

}

mat4 Camera::GetViewProjectionTransform() const
{
	return projectionMatrix * viewMatrix;
}

mat4 Camera::GetProjectionTransform() const
{
	return projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
	//Update view matrix
	viewMatrix = glm::lookAt( position, target, frame.up);
	
	//Update Camera frame
	frame.viewDir  = glm::normalize(target - position);
	frame.sideways = glm::normalize(glm::cross(frame.viewDir, frame.up ) );
}

const vec3& Camera::GetPosition() const
{
	return position;
}

void Camera::SetPosition(const vec3& pos)
{
	position = pos;
	UpdateViewMatrix();
}

void Camera::SetTarget(const vec3& pos)
{
	target = pos;
	UpdateViewMatrix();
}

void Camera::SetUpVector(const vec3& up)
{
	frame.up = up;
	UpdateViewMatrix();
}


void Camera::SetOrientation(const vec3& pos, const vec3& up)
{
	position = pos;
	frame.up = up;
	UpdateViewMatrix();
}

void Camera::SetOrientation2(const vec3& target, const vec3& up)
{
	this->target = target;
	this->frame.up = up;

	UpdateViewMatrix();
}

const vec3& Camera::GetTarget() const
{
	return target;
}

const CameraFrame& Camera::GetFrame() const
{
	return frame;
}

float Camera::GetNearPlane() const
{
	return nearP;
}

float Camera::GetFarPlane() const
{
	return farP;
}