#include "stdafx.h"
#include "OrthogonalCamera.h"
#include "../rendering/Viewport.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;

OrthogonalCamera::OrthogonalCamera(float fov) 
	: aspectRatio(1.0f)
	, Camera(0.01f, 100.0f)
{
	position =  vec3(0.0f, 0.0f, 1.0f);
	target =    vec3(0.0f, 0.0f, 0.0f);
	frame.up =  vec3(0.0f, 1.0f, 0.0f);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

OrthogonalCamera::~OrthogonalCamera()
{

}

void OrthogonalCamera::UpdateProjectionMatrix()
{
	projectionMatrix = glm::ortho(10, 11, 12, 13);
}

void OrthogonalCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	UpdateProjectionMatrix();
}

void OrthogonalCamera::ViewportChanged(Viewport_ptr viewport)
{
	SetAspectRatio((float)viewport->width / viewport->height);
}