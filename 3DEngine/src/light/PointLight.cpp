#include "stdafx.h"
#include "PointLight.h"

#include "../shape/Box.h"
#include "../shader/ConstShader.h"
#include "../util/Util.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

PointLight::PointLight(void)
	: Light()
{
	visMesh = Util::getBox();
	visMesh->init();

	visShader.reset(new ConstShader(color));

	visMesh->setShader(visShader);
}


PointLight::~PointLight(void)
{

}

void PointLight::SetPosition(vec4& pos)
{
	Light::SetPosition(pos);
	
	visMesh->worldTransform = glm::translate(mat4(1.0),vec3(pos.x,pos.y,pos.z));
	visMesh->worldTransform = glm::scale(visMesh->worldTransform,vec3(0.05));
}

void PointLight::SetColor(vec3& color)
{
	Light::SetColor(color);
	visShader->color = color;
}

void PointLight::Render(const Scene& scene)
{
	visMesh->render(scene);
}