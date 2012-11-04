#pragma once

#include "Camera.h"
#include "input/MouseCameraAdapter.h"

using glm::vec2;

class TestCamera :
	public Camera
{
public:
	TestCamera();
	~TestCamera();

	virtual void init();
	void setFov(float fov);
	void setAspectRatio(float aspectRatio);


protected:

	virtual void updateProjectionMatrix();

	float fov;
	float aspectRatio;

};

