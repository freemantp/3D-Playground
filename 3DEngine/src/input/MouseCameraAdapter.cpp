#include "stdafx.h"
#include "MouseCameraAdapter.h"
#include <iostream>

using glm::vec3;

MouseCameraAdapter::MouseCameraAdapter(Camera& cam) 
	: cam(cam)
{

}

MouseCameraAdapter::~MouseCameraAdapter(void)
{
}

void MouseCameraAdapter::onMouseDrag(const glm::vec2& pos)
{
	std:: cout <<"drag x: " << pos.x << " y:" << pos.y << std::endl;
	lastScreenPos = pos;
}

void MouseCameraAdapter::onMouseClick(Input::MouseButton button, Input::Direction state , const glm::vec2& pos)
{
	std:: cout << "click button=" << button << " state=" << state << " x=" << pos.x << " y=" << pos.y << std::endl;
	lastScreenPos = pos;
}

void MouseCameraAdapter::onMouseWheel(Input::Direction direction, const glm::vec2& pos)
{
	vec3 dir = cam.getCenter() - cam.getPosition();
	bool up = direction == Input::UP;

	float len = glm::length(dir);

	if(len <= cam.getNearPlane() && up || len >= cam.getFarPlane() && !up)
		return;
	
	dir = glm::normalize(dir) * 0.2f * (up ? 1.0f : -1.0f);
	cam.setPosition(cam.getPosition() + dir);

	lastScreenPos = pos;
}