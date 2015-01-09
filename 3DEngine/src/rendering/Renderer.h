#pragma once

#include "../util/SharedPointer.h"
#include "../input/ViewportObserver.h"

SHARED_PTR_CLASS_DECL(Renderer);
SHARED_PTR_CLASS_DECL(Viewport);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(GeometryBuffer);


class Renderer : public ViewportObserver
{
public:

	static Renderer_ptr Create(Viewport_ptr viewport);

	void Render();

	virtual void ViewportChanged(Viewport_ptr viewport) override;

	void SetScene(Scene_ptr scene);
	Scene_ptr Scene();

protected:
	Renderer(Viewport_ptr viewport);
	virtual ~Renderer();

	bool				m_ShowDebugElements;

	Scene_ptr			m_Scene;
	Scene_ptr			m_DebugScene;
	GeometryBuffer_ptr	m_gBuffer;
	Viewport_ptr		m_Viewport;

};

