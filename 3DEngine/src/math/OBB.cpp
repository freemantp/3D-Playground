#include "stdafx.h"

#include "OBB.h"

OBB::OBB()
{
	
}

OBB::OBB(const glm::vec3 & lower, const glm::vec3 & upper)
{
	p  = (upper + lower) / 2.0f;
	d = glm::abs(upper - p);
}
