#include "stdafx.h"

#include "BoundingBoxUtil.h"


#include <algorithm>  
#include <iterator> 

#include <glm/gtx/orthonormalize.hpp>

glm::mat3 BoundingBoxUtil::BasisFromDirection(const glm::vec3 & direction)
{
	auto directionNormalized = glm::normalize(direction);

	// Find directional component with least extent
	int minDirIndex = -1;
	float ldMin = std::numeric_limits<float>::max();
	for (int i = 0; i < 3; i++)
	{
		float current_comp = std::abs(directionNormalized[i]);
		if (current_comp < ldMin)
		{
			minDirIndex = i;
			ldMin = current_comp;
		}
	}
	assert(minDirIndex >= 0 && minDirIndex < 3);

	glm::vec3 sideVec0;
	sideVec0[minDirIndex] = 1;
	sideVec0 = glm::orthonormalize(sideVec0, directionNormalized);

	glm::vec3 sideVec1 = glm::cross(directionNormalized, sideVec0);
	return glm::mat3(directionNormalized, sideVec0, sideVec1);
}

bool BoundingBoxUtil::DirectionalLightFrustum(const AABBox & bbox, const glm::vec3& lightDir, OrthogonalFrustum& frust)
{
	auto get_corner = [&bbox](bool a, bool b, bool c)
	{
		glm::vec3 corner = bbox.p;
		corner[0] += (bbox.d[0] * (a ? 1.f : -1.f) );
		corner[1] += (bbox.d[1] * (b ? 1.f : -1.f) );
		corner[2] += (bbox.d[2] * (c ? 1.f : -1.f) );
		return corner;
	};

	glm::vec3 lightDirNormalized = glm::normalize(lightDir);

	bool has_comp[3];
	has_comp[0] = std::abs(lightDirNormalized[0]) > 1e-5;
	has_comp[1] = std::abs(lightDirNormalized[1]) > 1e-5;
	has_comp[2] = std::abs(lightDirNormalized[2]) > 1e-5;

	size_t num_corr = std::count(std::cbegin(has_comp), std::cend(has_comp), true);

	if (num_corr == 1)
	{
		auto it = std::find(std::cbegin(has_comp), std::cend(has_comp), true);
		int idx = static_cast<int>(std::distance(std::cbegin(has_comp),it));
		float sign = std::signbit(lightDirNormalized[idx]) ? 1.f : -1.f;

		frust.position = bbox.p;
		frust.position[idx] += (bbox.d[idx] * sign);
		frust.nearPlane = 0.0f;
		frust.farPlane = 2 * bbox.d[idx];

		int axis1 = (idx + 1) % 3;
		int axis2 = (idx + 2) % 3;

		frust.left = -bbox.d[axis1];
		frust.right = bbox.d[axis1];
		frust.up = bbox.d[axis2];
		frust.down = -bbox.d[axis2];

		frust.localCoordSys[0] = lightDir;
		glm::vec3 basisv1(0);
		basisv1[axis1] = 1;
		glm::vec3 basisv2(0);
		basisv2[axis2] = 1;

		frust.localCoordSys[1] = basisv1;
		frust.localCoordSys[2] = basisv2;

		return true;
	}
	else if (num_corr == 2 || num_corr == 3)
	{
		auto it = std::find(std::cbegin(has_comp), std::cend(has_comp), false);
		int axis0 = static_cast<int>(std::distance(std::cbegin(has_comp), it));
		int axis1 = (axis0 + 1) % 3;
		int axis2 = (axis0 + 2) % 3;

		frust.position = get_corner(has_comp[0], has_comp[1], has_comp[2]); // position
		frust.localCoordSys = BasisFromDirection(lightDirNormalized);

		auto project2plane = [&frust] (const glm::vec3& p, float& a, float& b)
		{
			glm::vec3 dv = p - frust.position;
			a = glm::dot(dv, frust.localCoordSys[1]);
			b = glm::dot(dv, frust.localCoordSys[2]);
		};

		float a_min = std::numeric_limits<float>::max();
		float b_min = std::numeric_limits<float>::max();
		float a_max = std::numeric_limits<float>::min();
		float b_max = std::numeric_limits<float>::min();

		float a, b;

		project2plane(get_corner(false, false, false), a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(false, false, true),  a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(false, true,  false), a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(false, true,  true),  a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(true,  false, false), a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(true,  false, true),  a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(true,  true,  false), a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);
		project2plane(get_corner(true,  true,  true),  a, b);
		a_min = std::min(a_min, a); b_min = std::min(b_min, b);
		a_max = std::max(a_max, a); b_max = std::max(b_max, b);

		frust.left = a_min;
		frust.right = a_max;
		frust.up = b_max;
		frust.down = b_min;
		frust.nearPlane = 0;
		frust.farPlane = glm::length(bbox.d) * 2.f;

		return true;

	}

	return false;
}
