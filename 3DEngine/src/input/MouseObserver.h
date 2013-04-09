#pragma once

#include <glm/glm.hpp>

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(MouseObserver);

namespace Input 
{
	enum MouseButton {LEFT, MIDDLE, RIGHT};
	enum Direction {UP, DOWN};
};

class MouseObserver
{
public:

	virtual void onMouseMove(const glm::vec2& position) = 0;
	virtual void onMouseDrag(const glm::vec2& position) = 0;
	virtual void onMouseClick(Input::MouseButton button, Input::Direction state, const glm::vec2& position) = 0;
	virtual void onMouseWheel(Input::Direction direction, const glm::vec2& position) = 0;
};