#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(DirectionalLight);
SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(Shadow);
SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(DepthTexture);

#include <glm/glm.hpp>

class Shadow
{
public:

	SHARED_PTR_FACTORY(Shadow);

	enum class ProjectionType { Perspective, Orthogonal};

	DepthTexture_ptr ShadowMap() const;

	const glm::mat4& BiasMatrix() const;
	const glm::mat4& ShadowMatrix() const;
	const glm::mat4& LightViewProjectionMatrix() const;
	float NearPlane() const;
	float FarPlane() const;
	ProjectionType Type() const { return type; };

	void UpdateShadowMatrix(const SpotLight_cptr& spotLight);
	void UpdateShadowMatrix(const DirectionalLight_cptr& dirLight, const Camera_cptr& camera);

protected:

	Shadow();

	DepthTexture_ptr shadowMapTex;

	glm::mat4 biasMatrix;
	glm::mat4 shadowMat;
	glm::mat4 depthViewProjectionMatrix;
	float nearPlane, farPlane;
	ProjectionType type;

};

