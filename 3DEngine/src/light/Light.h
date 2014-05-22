#pragma once;

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Light);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Shape);

class Scene;

class Light
{
public:

	Light();

	virtual glm::vec3& GetColor();
	virtual void SetColor(glm::vec3& pos);

	virtual Shape_ptr GetRepresentation() const;

protected:
	glm::vec3 color;


};