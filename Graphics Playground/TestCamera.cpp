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

void TestCamera::apply(void) {

}

void TestCamera::init(void) {

	vec4 position =  vec4(1.0f, 0.0f, 0.0f, 1.0f);
	viewMatrix = glm::lookAt( vec3(0.0f, 0.0f, 5.0f),
							 vec3(0.0f, 0.0f, 0.0f),
							 vec3(0.0f, 1.0f, 0.0f)
		);

	projectionMatrix  = glm::perspective(40.0f, 1.0f, 1.0f, 10.0f);
	
}