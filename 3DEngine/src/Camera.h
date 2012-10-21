#pragma once
#include <glm/glm.hpp>

using glm::mat4;

class Camera
{
public:
	virtual void init(void) = 0;

	mat4 getViewProjectionTransform() const
	{
		return projectionMatrix * viewMatrix;
	}

	mat4 viewMatrix;
	mat4 projectionMatrix;
};

