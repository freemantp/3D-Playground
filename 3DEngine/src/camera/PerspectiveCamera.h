#pragma once

#include "Camera.h"

using glm::vec2;

SHARED_PTR_CLASS_DECL(PerspectiveCamera);

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fov);
	~PerspectiveCamera();

	void SetFov(float fov);
	void SetAspectRatio(float aspectRatio);

	virtual void ViewportSizeChanged(int width, int height);

protected:

	virtual void UpdateProjectionMatrix();

	float fov;
	float aspectRatio;

};

