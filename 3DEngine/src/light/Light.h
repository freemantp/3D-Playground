#pragma once;

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Light);

class Scene;

using glm::vec3;
using glm::vec4;

class Light
{
public:

	Light();

	virtual void Render(const Scene& scene) = 0;

	virtual	vec4& GetPosition();
	virtual void SetPosition(vec4& pos);

	virtual vec3& GetColor();
	virtual void SetColor(vec3& pos);

protected:
	glm::vec3 color;
	glm::vec4 position;

};