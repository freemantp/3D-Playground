#include "stdafx.h"
#include "TestCamera.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;

TestCamera::TestCamera() 
	: fov(50.0f)
	, aspectRatio(1.0f)
	, nearP(1.0f)
	, farP(10.0f)
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


void TestCamera::updateViewMatrix()
{
	viewMatrix = glm::lookAt( position, center, up);
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

void TestCamera::setNearPlane(float nearP)
{
	this->nearP = nearP;
	updateProjectionMatrix();
}

void TestCamera::setFarPlane(float farP)
{
	this->farP = farP;
	updateProjectionMatrix();
}

void TestCamera::onMouseDrag(int x, int y)
{
	std:: cout <<"drag x: " << x << " y:" << y << std::endl;
}

void TestCamera::onMouseClick(Input::MouseButton button, Input::Direction state , int x, int y)
{
	std:: cout << "click button=" << button << " state=" << state << " x=" << x << " y=" << y << std::endl;
}

void TestCamera::onMouseWheel(Input::Direction direction, int x, int y)
{
	vec3 dir = center - position;
	bool up = direction == Input::UP;

	float len = glm::length(dir);

	if(len <= nearP && up || len >= farP && !up)
		return;
	
	dir = glm::normalize(dir) * 0.2f * (up ? 1.0f : -1.0f);
	position += dir;
	updateViewMatrix();
}