#pragma once

#include <glm/glm.hpp>

struct CameraFrame : public glm::mat3
{
	glm::vec3& ViewDir() { return operator[](0); };
	glm::vec3& Side() { return operator[](1); };
	glm::vec3& Up() { return operator[](2); };

	const glm::vec3& ViewDir() const { return operator[](0); };
	const glm::vec3& Side() const { return operator[](1); };
	const glm::vec3& Up() const { return operator[](2); };

	CameraFrame()
	{
		ViewDir() = glm::vec3(1, 0, 0);
		Side() = glm::vec3(0, 0, 1);
		Up() = glm::vec3(0, 1, 0);
	};
};

struct Frustum
{
	float nearPlane;
	float farPlane;
	glm::vec3 position;
	CameraFrame frame;
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