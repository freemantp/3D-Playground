#pragma once

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

	virtual void OnMouseMove(const glm::vec2& position) override;
	virtual void OnMouseDrag(const glm::vec2& position) override;
	virtual void OnMouseClick(Input::MouseButton button, Input::Direction state, const  glm::vec2& position) override;
	virtual void OnMouseWheel(Input::Direction direction, const glm::vec2& position) override;
	virtual void OnKey(const Input::Key key, const Input::Modifier mod, const glm::vec2& position) override;

protected:

	void Walk(float amount);
	void StepSidewards(float amount);
	void Turn(float degrees);

	glm::vec2 lastScreenPos;

};

