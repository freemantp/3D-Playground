#pragma once

#include "Light.h"

#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(Shape)
SHARED_PTR_CLASS_DECL(PointLight);
SHARED_PTR_CLASS_DECL(ConstColorShader);


class PointLight : public Light
{
public:
	
	SHARED_PTR_FACTORY(PointLight);

	virtual ~PointLight();
	virtual void SetPosition(glm::vec4& pos) override;
	virtual void SetColor(glm::vec3& color) override;
	virtual Shape_ptr GetRepresentation() const override;

protected:
	PointLight();

	Shape_ptr visMesh;
};

