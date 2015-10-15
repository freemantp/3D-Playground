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
	vec3 dir = glm::normalize( cam->Target() - cam->Position() );
	cam->SetPosition( cam->Position() + dir * amount );
}

void FirstPersonCameraAdapter::StepSidewards(float amount)
{
	vec3 step = cam->Frame().sideways * amount;
	cam->SetPosition( cam->Position() + step );
	cam->SetTarget( cam->Target() + step );	
}

void FirstPersonCameraAdapter::Turn(float degrees)
{

}

void FirstPersonCameraAdapter::OnMouseMove(const glm::vec2& position)
{
	const float degreesPerPixelYaw = 0.5f;
	const float degreesPerPixelPitch = 0.2f;

	const CameraFrame& frame = cam->Frame();
	const vec3& target = cam->Target();

	glm::vec2 screenDelta = position - lastScreenPos;
	float yaw = -screenDelta.x * degreesPerPixelYaw;
	float pitch = -screenDelta.y * degreesPerPixelPitch;

	//Transformation: translate to origin, rotate about axis, translate back
	glm::mat4 transformM = glm::translate(glm::mat4(1.0f),target);
	transformM = glm::rotate(transformM, glm::radians(pitch) , frame.sideways);
	transformM = glm::rotate(transformM, glm::radians(yaw), frame.up);
	transformM = glm::translate(transformM,-target);

	//Transform camera position
	glm::vec4 newDir = transformM * glm::vec4(frame.viewDir, 0.0f);
	glm::vec4 newUp = transformM * glm::vec4( frame.up, 0.0f);

	cam->SetOrientation2(cam->Position() + newDir.xyz,newUp.xyz);

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
