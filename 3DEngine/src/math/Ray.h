#pragma once

#include <glm/vec3.hpp>

class Ray
{
public:

	Ray(const glm::vec3& o, const glm::vec3& d);

	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inv_direction;
	int	sign[3];
};