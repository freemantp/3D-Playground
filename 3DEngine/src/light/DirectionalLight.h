#pragma once

#include "Light.h"
#include "../camera/Frustum.h"
#include "ShadowCastingLight.h"

SHARED_PTR_CLASS_DECL(Shadow);
SHARED_PTR_CLASS_DECL(DirectionalLight);

class DirectionalLight : public Light, public ShadowCastingLight, public std::enable_shared_from_this<DirectionalLight>
{
public:
	
	static DirectionalLight_ptr Create(const glm::vec3& direction,bool castsShadow = true);

	virtual const glm::vec3& Direction() const;
	virtual void SetDirection(const glm::vec3& direction);

	virtual void UpdateShadow() override;

	Shape_ptr ModelRepresentation() const override;

	void SetShadowFrustum(const OrthogonalFrustum& frust);

protected:

	DirectionalLight(const glm::vec3& direction,bool castsShadow);

	void UpdateVisMesh();

	OrthogonalFrustum shadowFrustum;

	glm::vec3 direction;
	Shape_ptr visMesh;

};

