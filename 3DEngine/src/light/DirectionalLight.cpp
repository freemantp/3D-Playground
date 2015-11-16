#include "stdafx.h"

#include "DirectionalLight.h"
#include "Shadow.h"

DirectionalLight_ptr DirectionalLight::Create(const glm::vec3& direction, bool castsShadow)
{
	DirectionalLight_ptr ptr = DirectionalLight_ptr(new DirectionalLight(direction,castsShadow));	
	ptr->UpdateShadow();
	return ptr;
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, bool castsShadow)
{
	if(castsShadow)
		shadow = Shadow::Create();

	this->direction = direction;
}

const glm::vec3& DirectionalLight::Direction() const
{
	return direction;
}

void DirectionalLight::SetDirection(const glm::vec3& dir)
{
	direction = glm::normalize(dir);	
	UpdateShadow();
}

void DirectionalLight::UpdateShadow()
{
	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}

