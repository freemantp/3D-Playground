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
	visMesh = Util::CreateBox();
	visMesh->Init();

	ConstantColorMaterial_ptr mat = ConstantColorMaterial::Create();
	mat->color = color;
	visMesh->SetMaterial(mat);
	visMesh->worldTransform = glm::scale(glm::mat4(1.0), glm::vec3(0.05f));
}


PointLight::~PointLight()
{

}

void PointLight::SetPosition(glm::vec4& pos)
{
	position = pos;
	visMesh->worldTransform[3] = position;
}

const glm::vec4& PointLight::Position() const
{
	return position;
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

Shape_ptr PointLight::ModelRepresentation() const
{
	return visMesh;
}