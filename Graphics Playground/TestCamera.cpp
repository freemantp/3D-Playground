#include "stdafx.h"
#include "TestCamera.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec4;
using glm::vec3;

TestCamera::TestCamera(void)
{
}

TestCamera::~TestCamera(void)
{

}

void TestCamera::init(void) 
{
	vec3 position =  vec3(0.0f, 0.0f, 10.0f);
	vec3 center =    vec3(0.0f, 0.0f, 0.0f);
	vec3 up =        vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt( position, center, up);

	projectionMatrix  = glm::perspective(60.0f, 1.0f, 1.0f, 10.0f);

}