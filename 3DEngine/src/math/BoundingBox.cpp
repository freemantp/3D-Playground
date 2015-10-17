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

glm::vec3 AABBox::LowerLeft() const
{
	return p-d;
}

glm::vec3 AABBox::UpperRight() const
{
	return p+d;
}

AABBox AABBox::Transform(const glm::mat4 & t) const
{
	glm::vec4 ll4(LowerLeft(), 1.f);
	glm::vec4 ur4(UpperRight(), 1.f);
	return AABBox((t * ll4).xyz(), (t * ur4).xyz());
}

AABBox AABBox::HullBox(const AABBox & other) const
{
	glm::vec3 ll, ur;

	auto l_lower = LowerLeft();
	auto r_lower = other.LowerLeft();
	auto l_upper = UpperRight();
	auto r_upper = other.UpperRight();

	for (int i = 0; i < 3; i++)
	{
		ll[i] = std::min(l_lower[i], r_lower[i]);
		ur[i] = std::max(l_upper[i], r_upper[i]);
	}

	return AABBox(ll,ur);
}

//inline AABBox AABBox::operator+(const AABBox & rhs)
//{
//	return HullBox(rhs);
//}

inline void AABBox::operator*=(const glm::mat4 & rhs)
{
	*this = Transform(rhs);
}

//inline void AABBox::operator+=(const AABBox & rhs)
//{
//	*this = HullBox(rhs);
//}

OBBox::OBBox()
{
	
}

OBBox::OBBox(const glm::vec3 & lower, const glm::vec3 & upper)
	: AABBox(lower, upper)
{

}

glm::vec3 OBBox::LowerLeft() const
{
	return (R * -d) + p;
}

glm::vec3 OBBox::UpperRight() const
{
	return (R * d) + p;
}
