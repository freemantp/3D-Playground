#include "stdafx.h"
#include "PerspectiveCamera.h"

using glm::vec4;
using glm::vec3;

PerspectiveCamera::PerspectiveCamera() 
	: fov(50.0f)
	, aspectRatio(1.0f)
	, Camera(1.0f, 10.0f)
{
	position =  vec3(0.0f, 0.0f, 2.0f);
	target =    vec3(0.0f, 0.0f, 0.0f);
	frame.up =  vec3(0.0f, 1.0f, 0.0f);

	updateViewMatrix();
	updateProjectionMatrix();
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::init() 
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
