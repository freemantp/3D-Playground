#pragma once

#include <glm/glm.hpp>
#include "Light.h"
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Shape)
SHARED_PTR_CLASS_DECL(PointLight);
class ConstShader;

class PointLight : public Light
{
public:
	PointLight(void);
	~PointLight(void);
	virtual void render(const Scene& scene);
	virtual void setPosition(glm::vec4& pos);
	virtual void setColor(glm::vec3& color);

protected:

	ConstShader* visShader;
	Shape_ptr visMesh;
};

