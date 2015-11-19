#pragma once

#include <glm/glm.hpp>
#include <array>

struct CameraFrame : public glm::mat3
{
	glm::vec3& ViewDir() { return operator[](0); };
	glm::vec3& Up() { return operator[](1); };
	glm::vec3& Side() { return operator[](2); };	

	const glm::vec3& ViewDir() const { return operator[](0); };
	const glm::vec3& Up() const { return operator[](1); };
	const glm::vec3& Side() const { return operator[](2); };	

	CameraFrame()
	{
		//OpenGL convention
		ViewDir() = glm::vec3(0, 0, -1);
		Side() = glm::vec3(1, 0, 0);
		Up() = glm::vec3(0, 1, 0);
	};

	CameraFrame(const glm::vec3& view, const glm::vec3& side, const glm::vec3& up)
		: glm::mat3(view, side, up) {};
};

struct Frustum
{
	virtual std::array<glm::vec3, 8> CornerPoints() = 0;

	float nearPlane;
	float farPlane;
	glm::vec3 position;
	CameraFrame frame;
};

struct OrthogonalFrustum : public Frustum
{
	virtual std::array<glm::vec3, 8> CornerPoints() override;

	float left;
	float right;
	float top;
	float bottom;
};

struct PerspectiveFrustum : public Frustum
{
	virtual std::array<glm::vec3, 8> CornerPoints() override;

	float fovY; // in degrees
	float aspectRatio;
};