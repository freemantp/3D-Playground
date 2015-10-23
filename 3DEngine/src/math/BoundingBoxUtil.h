#pragma once;

#include "BoundingBox.h"

#include <glm/glm.hpp>

class BoundingBoxUtil
{
public:
	static bool DirectionalLightFrustrum(const AABBox& box, const glm::vec3& lightDir, glm::vec3& light_pos, float& left, float& right, float&up, float& down, float& nearp, float& farp);
};