#include "stdafx.h"

#include "InspectionCameraAdapter.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


#include "Camera.h"

using glm::vec3;
using glm::vec4;

InspectionCameraAdapter::InspectionCameraAdapter(const Camera_ptr& cam) 
	: CameraAdapter(cam)
	, orbitCenter(glm::vec3(0,0,0))
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
			CameraFrame& frame = cam->Frame();

			//Calculate axis of rotation
			glm::vec3 rotation_axis = glm::normalize(-screen_delta.x * frame.Up() + -screen_delta.y * frame.Side());
			glm::mat4 transform_mat = glm::rotate(glm::mat4(1.0f), movement_len * pixel_rad_ratio, rotation_axis);

			//Transform camera position
			glm::vec3 new_pos = glm::vec3(transform_mat * glm::vec4(cam->Position(), 1.0f));
			glm::vec3 new_up  = glm::vec3(transform_mat * glm::vec4(frame.Up(), 0.0f));
			glm::vec3 new_dir = glm::vec3(transform_mat * glm::vec4(frame.ViewDir(), 0.0f));
	
			frame.Up() = new_up;
			frame.ViewDir() = glm::normalize(orbitCenter-new_pos);
			frame.Side() = glm::normalize(glm::cross(frame.ViewDir(), frame.Up()));
			cam->SetPosition(new_pos);
			cam->UpdateViewMatrix();
		}
		else if (lastButton == Input::MouseButton::RIGHT)
		{
			float dist = glm::length(orbitCenter - cam->Position());
			float pixel_length_ratio = 0.001f * dist;

			glm::vec3 offset = cam->Frame().Side() * -screen_delta.x * pixel_length_ratio;
			offset += cam->Frame().Up() * screen_delta.y * pixel_length_ratio;

			glm::vec3 new_pos = cam->Position() + offset;
			cam->SetPosition(new_pos);	

			orbitCenter += offset;
			cam->Frame().ViewDir() = glm::normalize(orbitCenter - new_pos);
			cam->UpdateViewMatrix();
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
	vec3 dir = orbitCenter - cam->Position();
	bool up = direction == Input::Direction::UP;

	float len = glm::length(dir);

	if(len <= cam->NearPlane() && up || len >= cam->FarPlane() && !up)
		return;
	
	dir = glm::normalize(dir) * 0.1f * (up ? 1.0f : -1.0f);
	cam->SetPosition(cam->Position() + dir);
	cam->UpdateViewMatrix();

	lastScreenPos = screen_pos;
}