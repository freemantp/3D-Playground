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
	
	virtual	const glm::vec4& Position() const;
	virtual void SetPosition(glm::vec4& pos);
	virtual void SetColor(glm::vec3& color) override;
	virtual Shape_ptr ModelRepresentation() const override;

protected:
	PointLight();

	virtual ~PointLight();

	glm::vec4 position;
	Shape_ptr visMesh;
};

