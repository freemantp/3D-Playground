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
	updateProjectioMatrix();
}

TestCamera::~TestCamera()
{

}

void TestCamera::init() 
{
	vec3 position =  vec3(0.0f, 0.0f, 2.0f);
	vec3 center =    vec3(0.0f, 0.0f, 0.0f);
	vec3 up =        vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt( position, center, up);

	updateProjectioMatrix();

}

void TestCamera::updateProjectioMatrix()
{
	projectionMatrix  = glm::perspective(fov, aspectRatio, nearP, farP);
}

void TestCamera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	updateProjectioMatrix();
}

void TestCamera::setFov(float fov)
{
	this->fov = fov;
	updateProjectioMatrix();
}

void TestCamera::setNearPlane(float nearP)
{
	this->nearP = nearP;
	updateProjectioMatrix();
}

void TestCamera::setFarPlane(float farP)
{
	this->farP = farP;
	updateProjectioMatrix();
}

void TestCamera::onMouseDrag(int x, int y)
{
	std:: cout <<"drag x: " << x << " y:" << y << std::endl;
}

void TestCamera::onMouseClick(Input::MouseButton button, Input::ButtonState state , int x, int y)
{
	std:: cout << "click button=" << button << " state=" << state << " x=" << x << " y=" << y << std::endl;
}

void TestCamera::onMouseWheel(int x, int y)
{
	std:: cout <<"wheel x: " << x << " y:" << y << std::endl;
}