#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Renderer);
SHARED_PTR_CLASS_DECL(Scene);

class Renderer
{
public:

	static Renderer_ptr Create();
	
	virtual ~Renderer();

	void Render();

	void SetScene(Scene_ptr scene);
	Scene_ptr Scene();

protected:
	Renderer();

	Scene_ptr m_Scene;

};

