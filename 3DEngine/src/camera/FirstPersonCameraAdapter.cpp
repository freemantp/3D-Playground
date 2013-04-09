#include "stdafx.h"
#include "FirstPersonCameraAdapter.h"
#include <iostream>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;


FirstPersonCameraAdapter::FirstPersonCameraAdapter(Camera_ptr cam)
	: CameraAdapter(cam)
{
}


FirstPersonCameraAdapter::~FirstPersonCameraAdapter(void)
{
}

void FirstPersonCameraAdapter::onKey(const Input::KEY key, const Input::MODIFIER mod, const glm::vec2& position)
{
	//std::cout << "Key " << key << " pressed" << std::endl;

	switch(key)
	{
		case Input::ARROW_UP: walk(0.2f); break;
		case Input::ARROW_DOWN: walk(-0.2f);  break;
		case Input::ARROW_LEFT: stepSidewards(-0.2f);  break;
		case Input::ARROW_RIGHT: stepSidewards(0.2f);  break;
	}
}

void FirstPersonCameraAdapter::walk(float amount)
{
	vec3 dir = glm::normalize( cam->getTarget() - cam->getPosition() );
	cam->setPosition( cam->getPosition() + dir * amount );
}

void FirstPersonCameraAdapter::stepSidewards(float amount)
{
	vec3 step = cam->getFrame().sideways * amount;
	cam->setPosition( cam->getPosition() + step );
	cam->setTarget( cam->getTarget() + step );	
}

void FirstPersonCameraAdapter::turn(float degrees)
{

}

void FirstPersonCameraAdapter::onMouseMove(const glm::vec2& position)
{
	const float degreesPerPixelYaw = 0.5f;
	const float degreesPerPixelPitch = 0.2f;

	const CameraFrame& frame = cam->getFrame();
	const vec3& target = cam->getTarget();

	vec2 screenDelta = position - lastScreenPos;
	float yaw = -screenDelta.x * degreesPerPixelYaw;
	float pitch = -screenDelta.y * degreesPerPixelPitch;

	//Transformation: translate to origin, rotate about axis, translate back
	glm::mat4 transformM = glm::translate(glm::mat4(1.0f),target);
	transformM = glm::rotate(transformM, pitch , frame.sideways);
	transformM = glm::rotate(transformM, yaw , frame.up);
	transformM = glm::translate(transformM,-target);

	//Transform camera position
	glm::vec4 newDir = transformM * glm::vec4(frame.viewDir, 0.0f);
	glm::vec4 newUp = transformM * glm::vec4( frame.up, 0.0f);

	cam->setOrientation2(cam->getPosition() + vec3(newDir.x,newDir.y,newDir.z),vec3(newUp.x,newUp.y,newUp.z));

	//cam->setOrientation(vec3(newPos.x,newPos.y,newPos.z), vec3(newUp.x,newUp.y,newUp.z));

	//std:: cout << position.x << " " << position.y << std::endl;

	lastScreenPos = position;
}

void FirstPersonCameraAdapter::onMouseDrag(const glm::vec2& screenPos)
{	

	lastScreenPos = screenPos;
}

void FirstPersonCameraAdapter::onMouseClick(Input::MouseButton button, Input::Direction state , const glm::vec2& screenPos)
{
	lastScreenPos = screenPos;
}

void FirstPersonCameraAdapter::onMouseWheel(Input::Direction direction, const glm::vec2& screenPos)
{

}
