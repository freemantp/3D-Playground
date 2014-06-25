#include "stdafx.h"

#include "DirectionalLight.h"
#include "Shadow.h"

DirectionalLight_ptr DirectionalLight::Create(bool castsShadow)
{
	DirectionalLight_ptr ptr = DirectionalLight_ptr(new DirectionalLight(castsShadow));
	ptr->UpdateShadow();
	return ptr;
}

DirectionalLight::DirectionalLight(bool castsShadow)
{
	shadow = Shadow::Create();
}

const glm::vec3& DirectionalLight::GetDirection() const
{
	return direction;
}

void DirectionalLight::SetDirection(glm::vec3& dir)
{
	direction = dir;
	
	UpdateShadow();
}

void DirectionalLight::UpdateShadow()
{
	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}

Shadow_ptr DirectionalLight::GetShadow() const
{
	return shadow;
}
