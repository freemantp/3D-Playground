#pragma once

#include "Camera.h"
#include "input/MouseObserver.h"

using glm::vec3;

class TestCamera :
	public Camera, public MouseObserver
{
public:
	TestCamera();
	~TestCamera();

	virtual void init();
	void setFov(float fov);
	void setAspectRatio(float aspectRatio);
	void setNearPlane(float near);
	void setFarPlane(float fov);

	virtual void onMouseDrag(int x, int y);
	virtual void onMouseClick(Input::MouseButton button, Input::Direction state,  int x, int y);
	virtual void onMouseWheel(Input::Direction direction, int x, int y);

protected:

	void updateProjectionMatrix();

	void updateViewMatrix();
	
	vec3 position;
	vec3 center;
	vec3 up;

	float fov;
	float aspectRatio;
	float nearP;
	float farP;
};

