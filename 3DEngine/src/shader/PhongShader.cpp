#include "stdafx.h"
#include "PhongShader.h"


PhongShader::PhongShader()
: ShaderBase()
{
	loadShader("phongShader");
}


PhongShader::~PhongShader(void)
{
}

void PhongShader::use(const Camera& cam, const glm::mat4& modelTransform)
{	

	vec4 lightPos(1,0,1,1);

	ShaderBase::use(cam,modelTransform);
	setUniform("Light.AmbientIntensity",  vec3(0.2) );	
	setUniform("Light.DiffuseIntensity",  vec3(1,1,1) );	
	setUniform("Light.SpecularIntensity", vec3(1,1,1) );	
	setUniform("Light.Position", cam.viewMatrix * lightPos );

	setUniform("Material.AmbientReflectivity", vec3(0.6) );
	setUniform("Material.DiffuseReflectivity", vec3(0,1,1) );
	setUniform("Material.SpecularReflectivity", vec3(1,0,1) );
	setUniform("Material.Shininess", 20);

}