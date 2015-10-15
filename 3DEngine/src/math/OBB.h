#pragma once;

#include <glm/glm.hpp>

/// Oriented bounding box
class OBB
{
public:

	OBB();

	OBB(const glm::vec3& lower, const glm::vec3& upper);

	glm::vec3 d;
	glm::vec3 p;
	glm::mat3 R;

};