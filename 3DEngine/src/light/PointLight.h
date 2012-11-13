#pragma once

#include <glm/glm.hpp>
#include "Light.h"

class Shape;

class PointLight : public Light
{
public:
	PointLight(void);
	~PointLight(void);
	virtual void render(const Scene& scene);

	glm::vec3 color;
	glm::vec4& getPosition();
	void setPosition(glm::vec4& pos);

protected:
	
	glm::vec4 position;

	Shape* visMesh;
};

