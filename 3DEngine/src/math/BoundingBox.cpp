#include "stdafx.h"

#include "BoundingBox.h"

#include <algorithm>

AABBox::AABBox()
{

}

AABBox::AABBox(const glm::vec3 & lower, const glm::vec3 & upper)
{
	p = (upper + lower) / 2.0f;
	d = glm::abs(upper - p);
}

glm::vec3 AABBox::Minimum() const
{
	return p-d;
}

glm::vec3 AABBox::Maximum() const
{
	return p+d;
}

AABBox AABBox::Transform(const glm::mat4 & t) const
{
	//See http://dev.theomader.com/transform-bounding-boxes/

	glm::vec3 bmin = Minimum();
	glm::vec3 bmax = Maximum();

	glm::vec3 basis_v[3] = { t[X].xyz(),  t[Y].xyz(), t[Z].xyz() };
	glm::vec3 transl = t[3].xyz();

	glm::vec3 bvX_a = basis_v[X] * bmin[X];
	glm::vec3 bvX_b = basis_v[X] * bmax[X];
	glm::vec3 bvY_a = basis_v[Y] * bmin[Y];
	glm::vec3 bvY_b = basis_v[Y] * bmax[Y];
	glm::vec3 bvZ_a = basis_v[Z] * bmin[Z];
	glm::vec3 bvZ_b = basis_v[Z] * bmax[Z];

	return AABBox(
		glm::min(bvX_a, bvX_b) + glm::min(bvY_a, bvY_b) + glm::min(bvZ_a, bvZ_b) + transl,
		glm::max(bvX_a, bvX_b) + glm::max(bvY_a, bvY_b) + glm::max(bvZ_a, bvZ_b) + transl
		);
}

AABBox AABBox::HullBox(const AABBox & other) const
{
	glm::vec3 ll, ur;

	auto l_lower = Minimum();
	auto r_lower = other.Minimum();
	auto l_upper = Maximum();
	auto r_upper = other.Maximum();

	for (int i = 0; i < 3; i++)
	{
		ll[i] = std::min(l_lower[i], r_lower[i]);
		ur[i] = std::max(l_upper[i], r_upper[i]);
	}

	return AABBox(ll,ur);
}

inline void AABBox::operator*=(const glm::mat4 & rhs)
{
	*this = Transform(rhs);
}

OBBox::OBBox()
{
	
}

OBBox::OBBox(const glm::vec3 & lower, const glm::vec3 & upper)
	: AABBox(lower, upper)
{

}

glm::vec3 OBBox::Minimum() const
{
	return (R * -d) + p;
}

glm::vec3 OBBox::Maximum() const
{
	return (R * d) + p;
}
