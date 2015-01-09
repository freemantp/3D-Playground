#pragma once

#include "Camera.h"

SHARED_PTR_CLASS_DECL(OrthogonalCamera);

class OrthogonalCamera : public Camera
{
public:
	OrthogonalCamera(float fov);
	~OrthogonalCamera();

	void SetAspectRatio(float aspectRatio);

	virtual void ViewportChanged(Viewport_ptr viewport) override;

protected:

	virtual void UpdateProjectionMatrix();

	float aspectRatio;

};

