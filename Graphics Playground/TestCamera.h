#pragma once

#include "Camera.h"
#include "input/MouseObserver.h"

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
	virtual void onMouseClick(Input::MouseButton button, Input::ButtonState state,  int x, int y);
	virtual void onMouseWheel(int x, int y);

protected:

	void updateProjectioMatrix();

	float fov;
	float aspectRatio;
	float nearP;
	float farP;
};

