#include "stdafx.h"
#include "PerspectiveCamera.h"
#include "../rendering/Viewport.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;

PerspectiveCamera::PerspectiveCamera(float fov) 
	: fov(fov)
	, aspectRatio(1.0f)
	, Camera(0.01f, 100.0f)
{
	position =  vec3(0.0f, 0.0f, 1.0f);
	target =    vec3(0.0f, 0.0f, 0.0f);
	frame.up =  vec3(0.0f, 1.0f, 0.0f);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::UpdateProjectionMatrix()
{
	projectionMatrix  = glm::perspective(glm::radians(fov), aspectRatio, nearP, farP);
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	UpdateProjectionMatrix();
}

void PerspectiveCamera::SetFov(float fov)
{
	this->fov = fov;
	UpdateProjectionMatrix();
}

void PerspectiveCamera::ViewportChanged(const Viewport_ptr& viewport)
{
	SetAspectRatio((float)viewport->width / viewport->height);
}