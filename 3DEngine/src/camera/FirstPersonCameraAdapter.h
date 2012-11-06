#pragma once

#include "Camera.h"
#include "CameraAdapter.h"

#include "../input/MouseObserver.h"
#include "../input/KeyboardObserver.h"

using glm::vec2;

class FirstPersonCameraAdapter : public CameraAdapter, public MouseObserver, public KeyboardObserver
{
public:
	FirstPersonCameraAdapter(Camera& cam);
	~FirstPersonCameraAdapter();

	virtual void onMouseDrag(const glm::vec2& position);
	virtual void onMouseClick(Input::MouseButton button, Input::Direction state, const  glm::vec2& position);
	virtual void onMouseWheel(Input::Direction direction, const glm::vec2& position);
	virtual void onKey(const Input::KEY key, const Input::MODIFIER mod, const glm::vec2& position);

protected:

	void walk(float amount);
	void stepSidewards(float amount);
	void turn(float degrees);

	vec2 lastScreenPos;

};

