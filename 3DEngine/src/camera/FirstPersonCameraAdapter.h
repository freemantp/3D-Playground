#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "CameraAdapter.h"
#include "../input/MouseObserver.h"
#include "../input/KeyboardObserver.h"

class KeyboardObserver;

class FirstPersonCameraAdapter : public CameraAdapter, public MouseObserver, public KeyboardObserver
{
public:
	FirstPersonCameraAdapter(Camera_ptr cam);
	~FirstPersonCameraAdapter();

	virtual void OnMouseMove(const glm::vec2& position);
	virtual void OnMouseDrag(const glm::vec2& position);
	virtual void OnMouseClick(Input::MouseButton button, Input::Direction state, const  glm::vec2& position);
	virtual void OnMouseWheel(Input::Direction direction, const glm::vec2& position);
	virtual void OnKey(const Input::KEY key, const Input::MODIFIER mod, const glm::vec2& position);

protected:

	void Walk(float amount);
	void StepSidewards(float amount);
	void Turn(float degrees);

	vec2 lastScreenPos;

};

