#pragma once;

#include "BoundingBox.h"
#include "BoundingBoxUtil.h"

#include "../camera/Frustum.h"

#include <glm/glm.hpp>

class BoundingBoxUtil
{
public:

	/// Creates a orthonormal basis from a given vector
	static CoordinateFrame BasisFromDirection(const glm::vec3& direction);

	/// Frustum
	static bool DirectionalLightFrustum(const AABBox& box, const glm::vec3& lightDir, OrthogonalFrustum& Frustum);

	/// Frustum
	static bool DirectionalLightFrustum(const AABBox& box, const glm::vec3& lightDir, const Frustum& camFrustum, OrthogonalFrustum& Frustum);
};