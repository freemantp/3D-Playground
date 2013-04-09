#include "stdafx.h"

#include "InspectionCameraAdapter.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.h"

using glm::vec3;
using glm::vec4;

InspectionCameraAdapter::InspectionCameraAdapter(Camera_ptr cam) 
	: CameraAdapter(cam)
{

}

InspectionCameraAdapter::~InspectionCameraAdapter(void)
{
}


void InspectionCameraAdapter::OnMouseMove(const glm::vec2& position)
{
	
}

void InspectionCameraAdapter::OnMouseDrag(const glm::vec2& screenPos)
{	
	//std:: cout <<"sp, x=" << screenPos.x << " y=" << screenPos.y << std::endl ;
	
	float degreeRatio = 0.25f;
	const CameraFrame& frame = cam->GetFrame();
	const vec3& target = cam->GetTarget();

	//direction of mouse movement on projetion plane
	vec2 screenDelta = screenPos - lastScreenPos;

	float len = glm::length(screenDelta);

	if(len <= 0.0f)
		return;

	//Calculate axis of rotation
	vec3 rotAxis = glm::normalize(-screenDelta.x * frame.up + -screenDelta.y * frame.sideways);

	//Transformation: translate to origin, rotate about axis, translate back
	glm::mat4 transformM = glm::translate(glm::mat4(1.0f),target);
	transformM = glm::rotate(transformM, len * degreeRatio, rotAxis);
	transformM = glm::translate(transformM,-target);

	//Transform camera position
	glm::vec4 newPos = transformM * glm::vec4(cam->GetPosition(), 1.0f);
	glm::vec4 newUp = transformM * glm::vec4( frame.up, 0.0f);
	cam->SetOrientation(vec3(newPos.x,newPos.y,newPos.z), vec3(newUp.x,newUp.y,newUp.z));

	lastScreenPos = screenPos;
}

void InspectionCameraAdapter::OnMouseClick(Input::MouseButton button, Input::Direction state , const glm::vec2& screenPos)
{
	//std:: cout << "click button=" << button << " state=" << state << " x=" << screenPos.x << " y=" << screenPos.y << std::endl;
	lastScreenPos = screenPos;
}

void InspectionCameraAdapter::OnMouseWheel(Input::Direction direction, const glm::vec2& screenPos)
{
	vec3 dir = cam->GetTarget() - cam->GetPosition();
	bool up = direction == Input::UP;

	float len = glm::length(dir);

	if(len <= cam->GetNearPlane() && up || len >= cam->GetFarPlane() && !up)
		return;
	
	dir = glm::normalize(dir) * 0.2f * (up ? 1.0f : -1.0f);
	cam->SetPosition(cam->GetPosition() + dir);

	lastScreenPos = screenPos;
}