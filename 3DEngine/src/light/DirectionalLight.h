#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();

	virtual const glm::vec3& GetDirection();
	virtual void SetDirection(glm::vec3& direction);


protected:

	glm::vec3 direction;



};

