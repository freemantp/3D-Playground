#pragma once

#include "Light.h"

SHARED_PTR_CLASS_DECL(Shadow);
SHARED_PTR_CLASS_DECL(DirectionalLight);

class DirectionalLight : public Light, public std::enable_shared_from_this<DirectionalLight>
{
public:
	
	static DirectionalLight_ptr Create(bool castsShadow = true);

	virtual const glm::vec3& GetDirection() const;
	virtual void SetDirection(glm::vec3& direction);
	
	Shadow_ptr Shadow() const;

protected:

	DirectionalLight(bool castsShadow);

	inline void UpdateShadow();

	glm::vec3 direction;
	Shadow_ptr shadow;

};

