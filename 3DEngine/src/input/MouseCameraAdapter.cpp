#include "stdafx.h"
#include "MouseCameraAdapter.h"
#include <iostream>

using glm::vec3;
using glm::vec4;

MouseCameraAdapter::MouseCameraAdapter(Camera& cam) 
	: cam(cam)
{

}

MouseCameraAdapter::~MouseCameraAdapter(void)
{
}

void MouseCameraAdapter::onMouseDrag(const glm::vec2& screenPos)
{	
	//std:: cout <<"sp, x=" << screenPos.x << " y=" << screenPos.y << std::endl ;
	
	float degreeRatio = 0.25f;
	const CameraFrame& frame = cam.getFrame();
	const vec3& camCenter = cam.getCenter();

	//direction of mouse movement on projetion plane
	vec2 screenDelta = screenPos - lastScreenPos;

	float len = glm::length(screenDelta);

	if(len <= 0.0f)
		return;

	//std:: cout <<"delta, x=" << screenDelta.x << " y=" << screenDelta.y << std::endl ;

	//Calculate axis of rotation
	vec3 rotAxis = glm::normalize(-screenDelta.x * frame.up + -screenDelta.y * frame.sideways);

	//Transformation: translate to origin, rotate about axis, translate back
	glm::mat4 transformM = glm::translate(glm::mat4(1.0f),camCenter);
	transformM = glm::rotate(transformM, len * degreeRatio, rotAxis);
	transformM = glm::translate(transformM,-camCenter);

	//Transform camera position
	glm::vec4 newPos = transformM * glm::vec4(cam.getPosition(),0.0f);
	glm::vec4 newUp = transformM * glm::vec4( frame.up, 0.0f);
	cam.setPosition(vec3(newPos.x,newPos.y,newPos.z), vec3(newUp.x,newUp.y,newUp.z));

	//std:: cout <<"dir, x=" << screenDelta.x << " y=" << screenDelta.y << std::endl;
	//std:: cout <<"rot, x=" << rotAxis.x << " y=" << rotAxis.y << " z=" << rotAxis.z << std::endl ;

	/*std:: cout <<"dir, x=" << frame.viewDir.x << " y=" << frame.viewDir.y << " z=" << frame.viewDir.z << std::endl ;
	std:: cout <<"up, x=" << frame.up.x << " y=" << frame.up.y << " z=" << frame.up.z << std::endl ;
	std:: cout <<"sid, x=" << frame.sideways.x << " y=" << frame.sideways.y << " z=" << frame.sideways.z << std::endl ;
	std:: cout <<"rot, x=" << rotAxis.x << " y=" << rotAxis.y << " z=" << rotAxis.z << std::endl ;
	std:: cout << std::endl;*/

	lastScreenPos = screenPos;
}

void MouseCameraAdapter::onMouseClick(Input::MouseButton button, Input::Direction state , const glm::vec2& screenPos)
{
	//std:: cout << "click button=" << button << " state=" << state << " x=" << screenPos.x << " y=" << screenPos.y << std::endl;
	lastScreenPos = screenPos;
}

void MouseCameraAdapter::onMouseWheel(Input::Direction direction, const glm::vec2& screenPos)
{
	vec3 dir = cam.getCenter() - cam.getPosition();
	bool up = direction == Input::UP;

	float len = glm::length(dir);

	if(len <= cam.getNearPlane() && up || len >= cam.getFarPlane() && !up)
		return;
	
	dir = glm::normalize(dir) * 0.2f * (up ? 1.0f : -1.0f);
	cam.setPosition(cam.getPosition() + dir, cam.getFrame().up);

	lastScreenPos = screenPos;
}