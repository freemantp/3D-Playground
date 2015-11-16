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
	glm::vec3 target = position + frame.ViewDir();
	viewMatrix = glm::lookAt( position, target, frame.Up());
}

const glm::vec3& Camera::Position() const
{
	return position;
}

void Camera::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

CameraFrame& Camera::Frame()
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