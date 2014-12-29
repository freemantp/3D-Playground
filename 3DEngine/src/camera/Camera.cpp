#include "stdafx.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float nearPlane, float farPlane)
	: nearP(nearPlane)
	, farP(farPlane)
{

}

glm::mat4 Camera::ViewProjectionTransform() const
{
	return projectionMatrix * viewMatrix;
}

glm::mat4 Camera::ProjectionTransform() const
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

const glm::vec3& Camera::Position() const
{
	return position;
}

void Camera::SetPosition(const glm::vec3& pos)
{
	position = pos;
	UpdateViewMatrix();
}

void Camera::SetTarget(const glm::vec3& pos)
{
	target = pos;
	UpdateViewMatrix();
}

void Camera::SetUpVector(const glm::vec3& up)
{
	frame.up = glm::normalize(up);
	UpdateViewMatrix();
}


void Camera::SetOrientation(const glm::vec3& pos, const glm::vec3& up)
{
	position = pos;
	frame.up = glm::normalize(up);
	UpdateViewMatrix();
}

void Camera::SetOrientation2(const glm::vec3& target, const glm::vec3& up)
{
	this->target = target;
	this->frame.up = glm::normalize(up);

	UpdateViewMatrix();
}

const glm::vec3& Camera::Target() const
{
	return target;
}

const CameraFrame& Camera::Frame() const
{
	return frame;
}

float Camera::NearPlane() const
{
	return nearP;
}

float Camera::FarPlane() const
{
	return farP;
}