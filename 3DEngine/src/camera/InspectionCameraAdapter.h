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
	InspectionCameraAdapter(Camera& cam);
	~InspectionCameraAdapter();

	virtual void onMouseMove(const glm::vec2& position);
	virtual void onMouseDrag(const glm::vec2& position);
	virtual void onMouseClick(Input::MouseButton button, Input::Direction state, const  glm::vec2& position);
	virtual void onMouseWheel(Input::Direction direction, const glm::vec2& position);

protected:

	vec2 lastScreenPos;

};

