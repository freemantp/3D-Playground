#include "stdafx.h"
#include "LightModel.h"
#include "../shader/UniformBuffer.h"
#include "../shader/UniformBufferShader.h"
#include "../light/PointLight.h"
#include "../camera/Camera.h"

LightModel::LightModel()
{
	
	UniformBufferShader* unformBufferShader = new UniformBufferShader();

	const int numElems = 8;
	const GLchar* elemNames[] = {"Lights.PointLights[0].Position","Lights.PointLights[0].Color",
								 "Lights.PointLights[1].Position","Lights.PointLights[1].Color",
								 "Lights.PointLights[2].Position","Lights.PointLights[2].Color",
								 "Lights.PointLights[3].Position","Lights.PointLights[3].Color"};
	
	lightsBuffer = new UniformBuffer(unformBufferShader,"Lights", elemNames, numElems);

}

LightModel::~LightModel()
{
	delete lightsBuffer;
}

UniformBuffer* LightModel::getLightsBuffer() const
{
	return lightsBuffer;
}

void LightModel::updateUniformBuffer(const Camera* cam)
{
	for(int i=0; i < pointLights.size(); i++)
	{
		PointLight* pl = pointLights[i];

		std::stringstream lightName;
		lightName << "Lights.PointLights[" << i << "].";
	
		lightsBuffer->setElement(lightName.str() + "Position", cam->viewMatrix * pl->getPosition() );
		lightsBuffer->setElement(lightName.str() + "Color", pl->getColor());
	}
}