#pragma once

#include "Camera.h"

using glm::vec2;

SHARED_PTR_CLASS_DECL(PerspectiveCamera);

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fov);
	~PerspectiveCamera();

	void setFov(float fov);
	void setAspectRatio(float aspectRatio);

	virtual void viewportSizeChanged(int width, int height);

protected:

	virtual void updateProjectionMatrix();

	float fov;
	float aspectRatio;

};

