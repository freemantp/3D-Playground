#include "stdafx.h"
#include "TestCamera.h"

using glm::vec4;
using glm::vec3;

TestCamera::TestCamera() 
	: fov(50.0f)
	, aspectRatio(1.0f)
	, Camera(1.0f, 10.0f)
{
	position =  vec3(0.0f, 0.0f, 2.0f);
	center =    vec3(0.0f, 0.0f, 0.0f);
	up =        vec3(0.0f, 1.0f, 0.0f);

	updateViewMatrix();
	updateProjectionMatrix();
}

TestCamera::~TestCamera()
{

}

void TestCamera::init() 
{

}

void TestCamera::updateProjectionMatrix()
{
	projectionMatrix  = glm::perspective(fov, aspectRatio, nearP, farP);
}

void TestCamera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	updateProjectionMatrix();
}

void TestCamera::setFov(float fov)
{
	this->fov = fov;
	updateProjectionMatrix();
}
