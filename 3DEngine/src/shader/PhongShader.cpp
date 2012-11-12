#include "stdafx.h"
#include "PhongShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"

PhongShader::PhongShader()
: ShaderBase()
{
	loadShader("phongShader");
	ambientReflection = vec3(0.0f);
	diffuseReflection = vec3(0.0f);
	glossyReflection = vec3(1.0f);
	shininess = 20;

}


PhongShader::~PhongShader(void)
{
}

void PhongShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	

	PointLight* pl = static_cast<PointLight*>(scene.lights[0]);

	ShaderBase::use(scene,modelTransform);
	
	setUniform("Light.AmbientIntensity",  vec3(0.0) );	
	setUniform("Light.DiffuseIntensity",  pl->color );	
	setUniform("Light.SpecularIntensity", pl->color );	
	setUniform("Light.Position", scene.activeCamera->viewMatrix * pl->position );

	setUniform("Material.AmbientReflectivity", ambientReflection );
	setUniform("Material.DiffuseReflectivity", diffuseReflection );
	setUniform("Material.SpecularReflectivity", glossyReflection );
	setUniform("Material.Shininess", shininess);
	
}