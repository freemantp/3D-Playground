#include "stdafx.h"

#include "DiffuseShader.h"
#include "UniformBuffer.h"

#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include <sstream>

DiffuseShader::DiffuseShader()
	: ShaderBase()
{
	loadShader("diffuseShader");
	materialColor = vec3(1,1,1);

	const int numElems = 8;
	const GLchar* elemNames[] = {"Lights.PointLights[0].Position","Lights.PointLights[0].Color",
								 "Lights.PointLights[1].Position","Lights.PointLights[1].Color",
								 "Lights.PointLights[2].Position","Lights.PointLights[2].Color",
								 "Lights.PointLights[3].Position","Lights.PointLights[3].Color"};

	lightsBuffer = new UniformBuffer(this,"Lights",elemNames,numElems);
}

DiffuseShader::~DiffuseShader()
{
	delete lightsBuffer;
}

void DiffuseShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	int numLights = (int)scene.lightModel.pointLights.size();
	
	setUniform("MaterialColor", materialColor);
	setUniform("NumLights", numLights);
	
	for(int i=0; i < numLights; i++)
	{

		PointLight* pl = scene.lightModel.pointLights[i];

		std::stringstream lightName;
		lightName << "Lights.PointLights[" << i << "].";
	
		lightsBuffer->setElement(lightName.str() + "Position", scene.activeCamera->viewMatrix * pl->getPosition() );
		lightsBuffer->setElement(lightName.str() + "Color",pl->getColor());
	}


	lightsBuffer->bindToShader(this,"Lights");
}