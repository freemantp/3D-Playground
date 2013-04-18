#include "stdafx.h"
#include "LightModel.h"
#include "../shader/UniformBuffer.h"
#include "../shader/UniformBufferShader.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../camera/Camera.h"

LightModel::LightModel()
	: valid(true)
{
	UniformBufferShader_ptr unformBufferShader(new UniformBufferShader());

	if(unformBufferShader->isLinked())
	{
		const int numElems = 28;
		const GLchar* elemNames[] = 
		{
			"Lights.PointLights[0].Position","Lights.PointLights[0].Color",
			"Lights.PointLights[1].Position","Lights.PointLights[1].Color",
			"Lights.PointLights[2].Position","Lights.PointLights[2].Color",
			"Lights.PointLights[3].Position","Lights.PointLights[3].Color",
			"Lights.SpotLights[0].Position","Lights.SpotLights[0].Color","Lights.SpotLights[0].Direction","Lights.SpotLights[0].CutoffAngle","Lights.SpotLights[0].Exponent",
			"Lights.SpotLights[1].Position","Lights.SpotLights[1].Color","Lights.SpotLights[1].Direction","Lights.SpotLights[1].CutoffAngle","Lights.SpotLights[1].Exponent",
			"Lights.SpotLights[2].Position","Lights.SpotLights[2].Color","Lights.SpotLights[2].Direction","Lights.SpotLights[2].CutoffAngle","Lights.SpotLights[2].Exponent",
			"Lights.SpotLights[3].Position","Lights.SpotLights[3].Color","Lights.SpotLights[3].Direction","Lights.SpotLights[3].CutoffAngle","Lights.SpotLights[3].Exponent",
		};

		lightsBuffer.reset(new UniformBuffer(unformBufferShader,"Lights", elemNames, numElems));

		valid = true;
	}

}

LightModel::~LightModel()
{
	
}

UniformBuffer_ptr LightModel::GetLightsBuffer() const
{
	return lightsBuffer;
}

void LightModel::UpdateUniformBuffer(Camera_cptr cam)
{
	
	glm::mat3 directionTransformMatrix	= glm::transpose(glm::inverse(glm::mat3(cam->viewMatrix)));
	
	for(int i=0; i < pointLights.size(); i++)
	{
		PointLight_ptr pl = pointLights[i];

		std::stringstream lightName;
		lightName << "Lights.PointLights[" << i << "].";
	
		lightsBuffer->setElement(lightName.str() + "Position", cam->viewMatrix * pl->GetPosition() );
		lightsBuffer->setElement(lightName.str() + "Color", pl->GetColor());
	}

	for(int i=0; i < spotLights.size(); i++)
	{
		SpotLight_ptr sl = spotLights[i];

		std::stringstream lightName;
		lightName << "Lights.SpotLights[" << i << "].";
	
		lightsBuffer->setElement(lightName.str() + "Position", cam->viewMatrix * sl->GetPosition() );
		lightsBuffer->setElement(lightName.str() + "Color", sl->GetColor());

		lightsBuffer->setElement(lightName.str() + "Direction", directionTransformMatrix * sl->GetDirection());
		lightsBuffer->setElement(lightName.str() + "CutoffAngle", sl->GetCutoffAngle());
		lightsBuffer->setElement(lightName.str() + "Exponent", sl->GetExponent());
	}

}

bool LightModel::IsValid()
{
	return valid;
}
