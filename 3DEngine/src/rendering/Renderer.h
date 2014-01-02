#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Renderer);
SHARED_PTR_CLASS_DECL(Viewport);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(GeometryBuffer);


class Renderer
{
public:

	static Renderer_ptr Create(Viewport_ptr viewport);
	
	virtual ~Renderer();

	void Render();

	void SetScene(Scene_ptr scene);
	Scene_ptr Scene();

protected:
	Renderer(Viewport_ptr viewport);

	Scene_ptr m_Scene;
	GeometryBuffer_ptr m_gBuffer;


};

