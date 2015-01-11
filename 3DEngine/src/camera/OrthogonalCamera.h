#pragma once

#include "Camera.h"

SHARED_PTR_CLASS_DECL(OrthogonalCamera);

class OrthogonalCamera : public Camera
{
public:
	OrthogonalCamera();
	~OrthogonalCamera();

	virtual void ViewportChanged(Viewport_ptr viewport) override;

protected:

	virtual void UpdateProjectionMatrix();

	int width;
	int height;

};

