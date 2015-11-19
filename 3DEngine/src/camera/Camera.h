#pragma once

#include "../input/ViewportObserver.h"
#include "../util/SharedPointer.h"

#include "Frustum.h"

#include <glm/glm.hpp>

class Camera : public ViewportObserver
{
public:

	glm::mat4 ViewProjectionTransform() const;

	glm::mat4 ProjectionTransform() const;

	void UpdateViewMatrix();
	virtual void UpdateProjectionMatrix() = 0;

	virtual Frustum& CameraFrustum() = 0;
		 
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

protected:
};

