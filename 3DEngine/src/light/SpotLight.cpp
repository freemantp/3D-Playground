#include "stdafx.h"
#include "SpotLight.h"


SpotLight::SpotLight(const vec3& direction, float cutoffAngle, float exponent)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
{

}


