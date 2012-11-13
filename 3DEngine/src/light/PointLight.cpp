#include "stdafx.h"
#include "PointLight.h"

#include "../shape/Box.h"
#include "../shader/ColorShader.h"
#include "../util/Util.h"

#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

PointLight::PointLight(void)
	: color(vec3(1,1,1))
{
	visMesh = Util::getBox();
	visMesh->init();

	visMesh->setShader(new ColorShader());
}


PointLight::~PointLight(void)
{
	delete visMesh;
}

vec4& PointLight::getPosition()
{
	return position;
}

void PointLight::setPosition(vec4& pos)
{
	position = pos;
	
	visMesh->worldTransform = glm::translate(mat4(1.0),vec3(pos.x,pos.y,pos.z));
	visMesh->worldTransform = glm::scale(visMesh->worldTransform,vec3(0.05));
}

void PointLight::render(const Scene& scene)
{
	visMesh->render(scene);
}