#pragma once

#include "../input/MouseObserver.h"
#include "CameraAdapter.h"

class Camera;
/**
* \class InspectionCameraAdapter for GLUT
* \brief Mouse adapter class that rotates around a reference point
* \author Michael Morandi
*
* A mouse adapter class that rotates around a reference point by holding down
* the left mouse button and dragging. The camera then rotates around its target point
*/
class InspectionCameraAdapter : public CameraAdapter, public MouseObserver
{
public:
	InspectionCameraAdapter(const Camera_ptr& cam);
	~InspectionCameraAdapter();

	virtual void OnMouseMove(const glm::vec2& position) override;
	virtual void OnMouseDrag(const glm::vec2& position) override;
	virtual void OnMouseClick(Input::MouseButton button, Input::Direction state, const glm::vec2& position) override;
	virtual void OnMouseWheel(Input::Direction direction, const glm::vec2& position) override;

protected:

	glm::vec2				lastScreenPos;
	Input::MouseButton		lastButton;
	Input::Direction		lastClickDirection;

};

