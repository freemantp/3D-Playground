#pragma once

#include <glm/glm.hpp>

struct Frustum
{
	float nearPlane;
	float farPlane;
	glm::vec3 position;
};

struct OrthogonalFrustum : public Frustum
{
	float left;
	float right;
	float up;
	float down;

	glm::mat3 localCoordSys; // column0 contains Frustum orientation (direction)
};

struct PerspectiveFrustum : public Frustum
{
	float fov;
	float aspectRatio;
	glm::vec3 direction;
};