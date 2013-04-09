#pragma once

#include <glm/glm.hpp>
#include "Light.h"
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Shape)
SHARED_PTR_CLASS_DECL(PointLight);
SHARED_PTR_CLASS_DECL(ConstShader);

class PointLight : public Light
{
public:
	PointLight(void);
	~PointLight(void);
	virtual void Render(const Scene& scene);
	virtual void SetPosition(glm::vec4& pos);
	virtual void SetColor(glm::vec3& color);

protected:

	ConstShader_ptr visShader;
	Shape_ptr visMesh;
};

