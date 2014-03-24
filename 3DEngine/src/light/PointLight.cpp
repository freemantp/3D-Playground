#include "stdafx.h"
#include "PointLight.h"

#include "../shape/Box.h"
#include "../util/Util.h"
#include "../materials/Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using glm::vec3;

PointLight::PointLight()
	: Light()
{
	visMesh = Util::GetBox();
	visMesh->Init();

	ConstantColorMaterial_ptr mat = ConstantColorMaterial::Create();
	mat->color = color;
	visMesh->SetMaterial(mat);
}


PointLight::~PointLight()
{

}

void PointLight::SetPosition(vec4& pos)
{
	__super::SetPosition(pos);
	
	visMesh->worldTransform = glm::translate(glm::mat4(1.0),vec3(pos.x,pos.y,pos.z));
	visMesh->worldTransform = glm::scale(visMesh->worldTransform,vec3(0.05f));
}

void PointLight::SetColor(vec3& color)
{
	Light::SetColor(color);
	ConstantColorMaterial_ptr mat = std::dynamic_pointer_cast<ConstantColorMaterial>(visMesh->GetMaterial());
	if (!mat)
	{
		mat = ConstantColorMaterial::Create();
		visMesh->SetMaterial(mat);
	}	

	mat->color = color;
}

Shape_ptr PointLight::GetRepresentation() const
{
	return visMesh;
}