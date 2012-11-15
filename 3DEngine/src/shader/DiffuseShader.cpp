#include "stdafx.h"

#include "DiffuseShader.h"
#include "UniformBuffer.h"

#include "../scene/Scene.h"
#include "../camera/Camera.h"

DiffuseShader::DiffuseShader()
	: ShaderBase()
{
	loadShader("diffuseShader");
	materialColor = vec3(1,1,1);

	const int numElems = 2;
	const GLchar* elemNames[] = {"LPosition","LColor"};
	lightsBuffer = new UniformBuffer(this,"PointLight",elemNames,numElems);

	vec4 lightPos = vec4(1.0f, 0.5f, 1.0f, 1.0f);
	vec3 lightColor = vec3(0.0f, 1.0f, 1.0f);
}

DiffuseShader::~DiffuseShader()
{
	delete lightsBuffer;
}

void DiffuseShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);
	
	if(scene.lightModel.pointLights.size() > 0)
	{
		PointLight* pl = scene.lightModel.pointLights[0];

		lightsBuffer->bindToShader(this,"PointLight");

		setUniform("MaterialColor", materialColor);

		
		lightsBuffer->setElement("LPosition", scene.activeCamera->viewMatrix * pl->getPosition() );
		lightsBuffer->setElement("LColor",pl->getColor());
		lightsBuffer->updateBufferObject();
		
	}
}