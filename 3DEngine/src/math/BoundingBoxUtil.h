#pragma once;

#include "BoundingBox.h"
#include "BoundingBoxUtil.h"

#include "../camera/Frustum.h"

#include <glm/glm.hpp>

class BoundingBoxUtil
{
public:

	/// Creates a orthonormal basis from a given vector
	static glm::mat3 BasisFromDirection(const glm::vec3& direction);

	/// Frustum
	static bool DirectionalLightFrustum(const AABBox& box, const glm::vec3& lightDir, OrthogonalFrustum& Frustum);
};