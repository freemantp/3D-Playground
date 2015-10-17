#include "stdafx.h"
#include "SpotLight.h"
#include "Shadow.h"

#include "../shape/WireCone.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext.hpp>

#include <iostream>

SpotLight_ptr SpotLight::Create(const glm::vec3& direction, float cutoffAngle, float exponent, bool castsShadow /*= true*/)
{
	return SpotLight_ptr(new SpotLight(direction, cutoffAngle, exponent, castsShadow));
}

SpotLight::SpotLight(const glm::vec3& direction, float cutoffAngle, float exponent, bool castsShadow)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
	, up(0,1,0)
{
	if (castsShadow)
		shadow = Shadow::Create();

	visMesh = WireCone::Create(cutoffAngle, 0.1f);
	visMesh->Init();
	UpdateVisMesh();
}

const glm::vec3& SpotLight::GetDirection() const
{ 
	return direction; 
}

const glm::vec3& SpotLight::GetUpVector() const
{
	return up;
}

void SpotLight::SetPosition(const glm::vec4& pos)
{
	__super::SetPosition(pos);
	UpdateShadow();
}

void SpotLight::SetDirection(const glm::vec3& dir)
{ 
	direction = glm::normalize(dir); 
	UpdateShadow();

	UpdateVisMesh();
}

void SpotLight::SetUpDirection(const glm::vec3& upVector)
{
	up = glm::normalize(upVector);
	UpdateShadow();
}


float SpotLight::CutoffAngle() const
{ 
	return cutoffAngle; 
}


void SpotLight::SetCutoffAngle(float theta)
{ 
	if (cutoffAngle != theta)
	{
		visMesh = WireCone::Create(theta, 0.1f);
		visMesh->Init();
	}
	
	cutoffAngle = theta;
}

void SpotLight::SetExponent(float exponent) 
{ 
	this->exponent = exponent; 
}

float SpotLight::Exponent() const
{ 
	return exponent; 
}

void SpotLight::UpdateShadow()
{
	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}

void SpotLight::UpdateVisMesh()
{
	const glm::vec3 z_vec(0, 0, 1);
	float rot_angle = std::acos(glm::dot(direction, z_vec));

	glm::vec3 xz_plane_dir = direction;
	xz_plane_dir.y = 0;
	xz_plane_dir = glm::normalize(xz_plane_dir);

	// How can a normalized vector not have length?? rounding? clamping is a workaround
	float pitch_angle = std::acos(glm::clamp(glm::dot(direction, xz_plane_dir), -1.f, 1.f));
	float yaw_angle = std::acos(glm::dot(xz_plane_dir, z_vec));

	if (xz_plane_dir.x < 0)
		yaw_angle = glm::two_pi<float>() - yaw_angle;

	visMesh->worldTransform = glm::yawPitchRoll(yaw_angle, pitch_angle, 0.f);
	visMesh->worldTransform[3] = position;
}

