#pragma once

#include <glm/glm.hpp>
#include "Light.h"

SHARED_PTR_CLASS_DECL(Shape)
SHARED_PTR_CLASS_DECL(PointLight);
SHARED_PTR_CLASS_DECL(ConstColorShader);


class PointLight : public Light
{
public:
	PointLight(void);
	~PointLight(void);
	virtual void Render(const Scene_ptr scene);
	virtual void SetPosition(glm::vec4& pos);
	virtual void SetColor(glm::vec3& color);

protected:

	Shape_ptr visMesh;
};

