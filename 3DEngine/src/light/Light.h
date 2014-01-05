#pragma once;

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Light);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Shape);

class Scene;

using glm::vec3;
using glm::vec4;

class Light
{
public:

	Light();

	virtual	vec4& GetPosition();
	virtual void SetPosition(vec4& pos);

	virtual vec3& GetColor();
	virtual void SetColor(vec3& pos);

	virtual Shape_ptr GetRepresentation() const;

protected:
	glm::vec3 color;
	glm::vec4 position;

};