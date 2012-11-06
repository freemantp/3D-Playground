#pragma once

#include <glm/glm.hpp>

class PointLight
{
public:
	PointLight(void);
	~PointLight(void);

	glm::vec3 color;
	glm::vec4 position;
};

