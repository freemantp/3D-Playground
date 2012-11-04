#pragma once

#include "../Camera.h"
#include "MouseObserver.h"

using glm::vec2;

class MouseCameraAdapter : public MouseObserver
{
public:
	MouseCameraAdapter(Camera& cam);
	~MouseCameraAdapter();

	virtual void onMouseDrag(const glm::vec2& position);
	virtual void onMouseClick(Input::MouseButton button, Input::Direction state, const  glm::vec2& position);
	virtual void onMouseWheel(Input::Direction direction, const glm::vec2& position);

protected:

	vec2 lastScreenPos;
	Camera& cam;

};

