#include "stdafx.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "FirstPersonCameraAdapter.h"
#include "Camera.h"

using glm::mat4;
using glm::vec3;

FirstPersonCameraAdapter::FirstPersonCameraAdapter(const Camera_ptr& cam)
	: CameraAdapter(cam)
{
}


FirstPersonCameraAdapter::~FirstPersonCameraAdapter(void)
{
}

void FirstPersonCameraAdapter::OnKey(const Input::Key key, const Input::Modifier mod, const glm::vec2& position)
{
	//std::cout << "Key " << key << " pressed" << std::endl;

	switch(key)
	{
		case Input::Key::ARROW_UP: Walk(0.2f); break;
		case Input::Key::ARROW_DOWN: Walk(-0.2f);  break;
		case Input::Key::ARROW_LEFT: StepSidewards(-0.2f);  break;
		case Input::Key::ARROW_RIGHT: StepSidewards(0.2f);  break;
	}
}

void FirstPersonCameraAdapter::Walk(float amount)
{
	//vec3 dir = glm::normalize( cam->Target() - cam->Position() );
	//cam->SetPosition( cam->Position() + dir * amount );
}

void FirstPersonCameraAdapter::StepSidewards(float amount)
{
	auto& frustum = cam->CameraFrustum();
	vec3 step = frustum.frame.Side() * amount;
	frustum.position += step;
	cam->UpdateViewMatrix();
}

void FirstPersonCameraAdapter::Turn(float degrees)
{

}

void FirstPersonCameraAdapter::OnMouseMove(const glm::vec2& position)
{
	const float degreesPerPixelYaw = 0.5f;
	const float degreesPerPixelPitch = 0.2f;

	auto& frustum = cam->CameraFrustum();
	vec3 target;// = cam->Target();

	glm::vec2 screenDelta = position - lastScreenPos;
	float yaw = -screenDelta.x * degreesPerPixelYaw;
	float pitch = -screenDelta.y * degreesPerPixelPitch;

	//Transformation: translate to origin, rotate about axis, translate back
	glm::mat4 transformM = glm::translate(glm::mat4(1.0f),target);
	transformM = glm::rotate(transformM, glm::radians(pitch) , frustum.frame.Side());
	transformM = glm::rotate(transformM, glm::radians(yaw), frustum.frame.Up());
	transformM = glm::translate(transformM,-target);

	//Transform camera position
	glm::vec4 newDir = transformM * glm::vec4(frustum.frame.ViewDir(), 0.0f);
	glm::vec4 newUp = transformM * glm::vec4(frustum.frame.Up(), 0.0f);
	lastScreenPos = position;
}

void FirstPersonCameraAdapter::OnMouseDrag(const glm::vec2& screenPos)
{	
	lastScreenPos = screenPos;
}

void FirstPersonCameraAdapter::OnMouseClick(Input::MouseButton button, Input::Direction state , const glm::vec2& screenPos)
{
	lastScreenPos = screenPos;
}

void FirstPersonCameraAdapter::OnMouseWheel(Input::Direction direction, const glm::vec2& screenPos)
{

}
