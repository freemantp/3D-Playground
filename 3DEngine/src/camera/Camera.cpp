#include "stdafx.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

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
	Frustum& frustum = CameraFrustum();
	glm::vec3 target = frustum.position + frustum.frame.ViewDir();
	viewMatrix = glm::lookAt(frustum.position, target, frustum.frame.Up());
	NotifyObservers();
}

void Camera::AddObserver(const CameraObserver_ptr & observer)
{
	observers.push_back(observer);
}

void Camera::NotifyObservers()
{	
	for (auto& obs : observers)	
		obs->CameraChanged();	
}
