#pragma once

#include <glm/vec3.hpp>
#include <array>

#include "Frame.h"
	

struct Frustum
{
	Frustum();

	/// Returns the corner points of a view frustum (just orientation, not position considered)
	virtual std::array<glm::vec3, 8> CornerPoints() const = 0;

	float nearPlane;
	float farPlane;
	glm::vec3 position;
	CoordinateFrame frame;
};

struct OrthogonalFrustum : public Frustum
{
	virtual std::array<glm::vec3, 8> CornerPoints() const override;

	float left;
	float right;
	float top;
	float bottom;
};

struct PerspectiveFrustum : public Frustum
{
	virtual std::array<glm::vec3, 8> CornerPoints() const override;

	float fovY; // in degrees
	float aspectRatio;
};