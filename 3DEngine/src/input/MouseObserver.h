#pragma once

#include <glm/glm.hpp>

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(MouseObserver);

namespace Input 
{
	enum class MouseButton {LEFT, MIDDLE, RIGHT};
	enum class Direction {UP, DOWN};
};

class MouseObserver
{
public:

	virtual void OnMouseMove(const glm::vec2& position) = 0;
	virtual void OnMouseDrag(const glm::vec2& position) = 0;
	virtual void OnMouseClick(Input::MouseButton button, Input::Direction state, const glm::vec2& position) = 0;
	virtual void OnMouseWheel(Input::Direction direction, const glm::vec2& position) = 0;
};