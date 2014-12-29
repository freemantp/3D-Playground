#include "stdafx.h"

#include "InspectionCameraAdapter.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
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

void InspectionCameraAdapter::OnMouseDrag(const glm::vec2& screen_pos)
{	
	glm::vec2 screen_delta = screen_pos - lastScreenPos;
	float movement_len = glm::length(screen_delta);

	if (movement_len > 0.0f)
	{
		//std:: cout <<"sp, x=" << screenPos.x << " y=" << screenPos.y << std::endl ;

		if (lastButton == Input::MouseButton::LEFT)
		{
			const float pixel_rad_ratio = 0.004f;
			const CameraFrame& frame = cam->Frame();
			const vec3& target = cam->Target();

			//Calculate axis of rotation
			glm::vec3 rotation_axis = glm::normalize(-screen_delta.x * frame.up + -screen_delta.y * frame.sideways);

			//Transformation: translate to origin, rotate about axis, translate back
			glm::mat4 transform_mat = glm::translate(glm::mat4(1.0f), target);
			transform_mat = glm::rotate(transform_mat, movement_len * pixel_rad_ratio, rotation_axis);
			transform_mat = glm::translate(transform_mat, -target);

			//Transform camera position
			glm::vec4 new_pos = transform_mat * glm::vec4(cam->Position(), 1.0f);
			glm::vec4 new_up = transform_mat * glm::vec4(frame.up, 0.0f);
			cam->SetOrientation(glm::vec3(new_pos.x, new_pos.y, new_pos.z), vec3(new_up.x, new_up.y, new_up.z));	
		}
		else if (lastButton == Input::MouseButton::RIGHT)
		{
			float dist = glm::length(cam->Target() - cam->Position());
			float pixel_length_ratio = 0.001f * dist;

			glm::vec3 offset = cam->Frame().sideways * -screen_delta.x * pixel_length_ratio;
			offset += cam->Frame().up * screen_delta.y * pixel_length_ratio;
			cam->SetPosition(cam->Position() + offset);
			cam->SetTarget(cam->Target() + offset);
		}

		lastScreenPos = screen_pos;
	}
}

void InspectionCameraAdapter::OnMouseClick(Input::MouseButton button, Input::Direction direction , const glm::vec2& screen_pos)
{
	//std:: cout << "click button=" << button << " state=" << state << " x=" << screenPos.x << " y=" << screenPos.y << std::endl;
	lastScreenPos = screen_pos;
	lastClickDirection = direction;
	lastButton = button;
}

void InspectionCameraAdapter::OnMouseWheel(Input::Direction direction, const glm::vec2& screen_pos)
{
	vec3 dir = cam->Target() - cam->Position();
	bool up = direction == Input::Direction::UP;

	float len = glm::length(dir);

	if(len <= cam->NearPlane() && up || len >= cam->FarPlane() && !up)
		return;
	
	dir = glm::normalize(dir) * 0.2f * (up ? 1.0f : -1.0f);
	cam->SetPosition(cam->Position() + dir);

	lastScreenPos = screen_pos;
}