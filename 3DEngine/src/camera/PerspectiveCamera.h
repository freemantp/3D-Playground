#pragma once

#include "Camera.h"

using glm::vec2;

class PerspectiveCamera :
	public Camera
{
public:
	PerspectiveCamera();
	~PerspectiveCamera();

	virtual void init();
	void setFov(float fov);
	void setAspectRatio(float aspectRatio);


protected:

	virtual void updateProjectionMatrix();

	float fov;
	float aspectRatio;

};

