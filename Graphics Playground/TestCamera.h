#pragma once

#include "Camera.h"

class TestCamera :
	public Camera
{
public:
	TestCamera(void);
	~TestCamera(void);

	virtual void init(void);
	void setFov(float fov);
	void setAspectRatio(float aspectRatio);
	void setNearPlane(float near);
	void setFarPlane(float fov);

protected:

	void updateProjectioMatrix(void);

	float fov;
	float aspectRatio;
	float nearP;
	float farP;
};

