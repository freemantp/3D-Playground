#pragma once

#include "Light.h"
#include "ShadowCastingLight.h"

SHARED_PTR_CLASS_DECL(Shadow);
SHARED_PTR_CLASS_DECL(DirectionalLight);

class DirectionalLight : public Light, public ShadowCastingLight, public std::enable_shared_from_this<DirectionalLight>
{
public:
	
	static DirectionalLight_ptr Create(bool castsShadow = true);

	virtual const glm::vec3& Direction() const;
	virtual void SetDirection(glm::vec3& direction);

	virtual void UpdateShadow() override;

protected:

	DirectionalLight(bool castsShadow);

	glm::vec3 direction;

};

