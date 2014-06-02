#pragma once

#include "Light.h"

SHARED_PTR_CLASS_DECL(DirectionalLight);

class DirectionalLight : public Light
{
public:
	
	static DirectionalLight_ptr Create();

	virtual const glm::vec3& GetDirection();
	virtual void SetDirection(glm::vec3& direction);


protected:

	DirectionalLight();

	glm::vec3 direction;



};

