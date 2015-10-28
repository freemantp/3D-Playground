#include "stdafx.h"

#include "BoundingBoxUtil.h"

#include <algorithm>  
#include <iterator> 

bool BoundingBoxUtil::DirectionalLightFrustrum(const AABBox & bbox, const glm::vec3& lightDir2, glm::vec3 & light_pos, float & left, float & right, float & up, float & down, float & nearp, float & farp)
{
	auto project2plane = [](const glm::vec3& pop, const glm::vec3& n, const glm::vec3& p) -> glm::vec3
	{
		float projection_len = glm::dot(n, p - pop);
		return p - projection_len * n;
	};


	glm::vec3 lightDir = glm::normalize(lightDir2);

	float x_corr = glm::dot(lightDir, glm::vec3(1, 0, 0));
	float y_corr = glm::dot(lightDir, glm::vec3(0, 1, 0));
	float z_corr = glm::dot(lightDir, glm::vec3(0, 0, 1));

	bool has_comp[3];
	has_comp[0] = std::abs(x_corr) > 1e-5;
	has_comp[1] = std::abs(y_corr) > 1e-5;
	has_comp[2] = std::abs(z_corr) > 1e-5;

	size_t num_corr = std::count(std::cbegin(has_comp), std::cend(has_comp), true);

	if (num_corr == 1)
	{
		auto it = std::find(std::cbegin(has_comp), std::cend(has_comp), true);
		int idx = static_cast<int>(std::distance(std::cbegin(has_comp),it));
		float sign = std::signbit(lightDir[idx]) ? 1.f : -1.f;

		light_pos = bbox.p;
		light_pos[idx] += (bbox.d[idx] * sign);
		nearp = 0.0f;
		farp = 2 * bbox.d[idx];

		int axis1 = (idx + 1) % 3;
		int axis2 = (idx + 2) % 3;

		left = -bbox.d[axis1];
		right = bbox.d[axis1];
		up = bbox.d[axis2];
		down = -bbox.d[axis2];

		return true;
	}
	else if (num_corr == 3)
	{
		auto it = std::find(std::cbegin(has_comp), std::cend(has_comp), false);
		int idx = static_cast<int>(std::distance(std::cbegin(has_comp), it));

		int axis1 = (idx + 1) % 3;
		int axis2 = (idx + 2) % 3;

		float sign1 = std::signbit(lightDir[axis1]) ? 1.f : -1.f;
		float sign2 = std::signbit(lightDir[axis2]) ? 1.f : -1.f;

		glm::vec3 p = bbox.p;
		p[axis1] += bbox.d[axis1];
		p[axis2] += bbox.d[axis2];

		glm::vec3 p2p1 = bbox.p + bbox.d;

	}
	else if (num_corr == 2)
	{
		
	}

	return false;
}
