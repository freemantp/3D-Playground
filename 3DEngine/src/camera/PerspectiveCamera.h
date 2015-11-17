#pragma once

#include "Camera.h"

SHARED_PTR_CLASS_DECL(PerspectiveCamera);

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fov);

	void SetFov(float fov);
	void SetAspectRatio(float aspectRatio);

	virtual void ViewportChanged(const Viewport_ptr& viewport) override;

	virtual Frustum& CameraFrustum() override;

protected:

	virtual void UpdateProjectionMatrix();

	PerspectiveFrustum frustum;
};

