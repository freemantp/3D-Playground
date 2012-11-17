#include "stdafx.h"
#include "PhongTextureShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"

PhongTextureShader::PhongTextureShader()
: PhongShader("phongTexShader")
{

}


PhongTextureShader::~PhongTextureShader()
{

}

void PhongTextureShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	PhongShader::use(scene,modelTransform);
}