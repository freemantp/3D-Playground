#include "stdafx.h"

#include "DirectionalLight.h"
#include "Shadow.h"

#include "../shape/Box.h"
#include "../util/Util.h"
#include "../materials/Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	visMesh = Util::CreateWireBox();

	ConstantColorMaterial_ptr mat = ConstantColorMaterial::Create();
	mat->color = color;
	visMesh->SetMaterial(mat);
	visMesh->SetWorldTransform(glm::scale(glm::mat4(1.0), glm::vec3(0.f)));
}

void DirectionalLight::UpdateVisMesh()
{
	glm::mat4 tmat = glm::translate(glm::mat4(1), shadowFrustum.position);

	tmat *= glm::mat4(shadowFrustum.frame);

	tmat = glm::translate(tmat, glm::vec3(shadowFrustum.nearPlane,
		shadowFrustum.top - (shadowFrustum.top - shadowFrustum.bottom) / 2.f,
		shadowFrustum.left - (shadowFrustum.left - shadowFrustum.right) / 2.f));

	tmat = glm::scale(tmat, glm::vec3(shadowFrustum.farPlane - shadowFrustum.nearPlane,
		(shadowFrustum.top - shadowFrustum.bottom) / 2, 
		(shadowFrustum.left - shadowFrustum.right) / 2));

	tmat = glm::translate(tmat, glm::vec3(1,0,0));

	visMesh->SetWorldTransform(tmat);
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
	if (shadow && camera)
		shadow->UpdateShadowMatrix(shared_from_this(),camera);
}

Shape_ptr DirectionalLight::ModelRepresentation() const
{
	return visMesh;
}

void DirectionalLight::SetShadowFrustum(const OrthogonalFrustum & frust)
{
	shadowFrustum = frust;
	UpdateVisMesh();
}

