#pragma once

#include <glm/glm.hpp>
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(void);
	~PointLight(void);

	glm::vec3 color;
	glm::vec4 position;
};

