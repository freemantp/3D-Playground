#pragma once;

#include <glm/glm.hpp>

class Scene;

using glm::vec3;
using glm::vec4;

class Light
{
public:

	Light();

	virtual void render(const Scene& scene) = 0;

	virtual	vec4& getPosition();
	virtual void setPosition(vec4& pos);

	virtual vec3& getColor();
	virtual void setColor(vec3& pos);

protected:
	glm::vec3 color;
	glm::vec4 position;

};