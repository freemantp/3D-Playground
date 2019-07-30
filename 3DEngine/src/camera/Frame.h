#pragma once

#include <glm/fwd.hpp>
#include <glm/mat3x3.hpp>

struct CoordinateFrame : public glm::mat3
{
	enum Axis {ViewAxis=0, UpAxis=1, SideAxis=2};

	glm::vec3& ViewDir();
	glm::vec3& Up();
	glm::vec3& Side();

	const glm::vec3& ViewDir() const;
	const glm::vec3& Up() const;
	const glm::vec3& Side() const;

	/// Creates an OpenGL  coordinate system (view direction is along negative Z)
	CoordinateFrame();

	CoordinateFrame(const glm::vec3& view, const glm::vec3& side, const glm::vec3& up);
};