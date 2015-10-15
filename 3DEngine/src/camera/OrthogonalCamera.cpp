#include "stdafx.h"
#include "OrthogonalCamera.h"
#include "../rendering/Viewport.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;

OrthogonalCamera::OrthogonalCamera() 
	: Camera(0.01f, 10.0f)
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
	projectionMatrix = glm::ortho(	0.f, static_cast<float>(width),
									 static_cast<float>(height), 0.f,
									nearP, farP);
}

void OrthogonalCamera::ViewportChanged(const Viewport_ptr& viewport)
{
	width = viewport->width;
	height = viewport->height;
	UpdateProjectionMatrix();
}