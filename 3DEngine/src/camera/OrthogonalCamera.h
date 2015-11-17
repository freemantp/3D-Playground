#pragma once

#include "Camera.h"

SHARED_PTR_CLASS_DECL(OrthogonalCamera);

class OrthogonalCamera : public Camera
{
public:
	OrthogonalCamera();
	~OrthogonalCamera();

	virtual void ViewportChanged(const Viewport_ptr& viewport) override;

	virtual Frustum& CameraFrustum() override;

protected:

	virtual void UpdateProjectionMatrix();

	OrthogonalFrustum frustum;
};

