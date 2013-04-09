#pragma once

#include <glm/glm.hpp>

namespace Input 
{
	enum KEY {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,ARROW_UP,ARROW_DOWN,ARROW_LEFT,ARROW_RIGHT,SPACE,ESCAPE};
	enum MODIFIER {NONE,SHIFT,CONTROL, ALT};
};

class KeyboardObserver
{
public:

	virtual void OnKey(const Input::KEY key, const Input::MODIFIER mod, const glm::vec2& position) = 0;
};