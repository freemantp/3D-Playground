#include "stdafx.h"
#include "PerspectiveCamera.h"
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

	updateViewMatrix();
	updateProjectionMatrix();
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::updateProjectionMatrix()
{
	projectionMatrix  = glm::perspective(fov, aspectRatio, nearP, farP);
}

void PerspectiveCamera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	updateProjectionMatrix();
}

void PerspectiveCamera::setFov(float fov)
{
	this->fov = fov;
	updateProjectionMatrix();
}

void PerspectiveCamera::viewportSizeChanged(int width, int height)
{
	setAspectRatio((float)width / height);
}