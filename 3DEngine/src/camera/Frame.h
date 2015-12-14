#pragma once

#include <glm/glm.hpp>

struct CoordinateFrame : public glm::mat3
{
	enum Axis {ViewAxis=0, UpAxis=1, SideAxis=2};

	glm::vec3& ViewDir() { return operator[](ViewAxis); };
	glm::vec3& Up() { return operator[](UpAxis); };
	glm::vec3& Side() { return operator[](SideAxis); };

	const glm::vec3& ViewDir() const { return operator[](ViewAxis); };
	const glm::vec3& Up() const { return operator[](UpAxis); };
	const glm::vec3& Side() const { return operator[](SideAxis); };

	/// Creates an OpenGL  coordinate system (view direction is along negative Z)
	CoordinateFrame()
	{
		//OpenGL convention
		ViewDir() = glm::vec3(0, 0, -1);
		Up() = glm::vec3(0, 1, 0);
		Side() = glm::vec3(1, 0, 0);	
	};

	CoordinateFrame(const glm::vec3& view, const glm::vec3& side, const glm::vec3& up)
		: glm::mat3(view, up, side) {};
};