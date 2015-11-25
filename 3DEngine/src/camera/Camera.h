#pragma once

#include "../input/ViewportObserver.h"
#include "../util/SharedPointer.h"

#include "Frustum.h"

#include <glm/glm.hpp>
#include <vector>

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(CameraObserver);

class CameraObserver
{
public:
	virtual void CameraChanged() = 0;
};

class Camera : public ViewportObserver
{
public:

	glm::mat4 ViewProjectionTransform() const;

	glm::mat4 ProjectionTransform() const;

	void UpdateViewMatrix();
	virtual void UpdateProjectionMatrix() = 0;

	virtual Frustum& CameraFrustum() = 0;

	virtual const Frustum& CameraFrustum() const = 0;
		 
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	void AddObserver(const CameraObserver_ptr& observer);

protected:

	void NotifyObservers();

	std::vector<CameraObserver_ptr> observers;
};

