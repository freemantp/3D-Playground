#pragma once

#include "../util/SharedPointer.h"
#include "../input/ViewportObserver.h"
#include "../input/KeyboardObserver.h"

SHARED_PTR_CLASS_DECL(Renderer);
SHARED_PTR_CLASS_DECL(Viewport);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(GeometryBuffer);


class Renderer : public ViewportObserver, public KeyboardObserver
{
public:

	static Renderer_ptr Create(const Viewport_ptr& viewport);

	void Render();

	virtual void ViewportChanged(const Viewport_ptr& viewport) override;

	virtual void OnKey(const Input::Key key, const Input::Modifier mod) override;

	void SetScene(Scene_ptr scene);

	Scene_ptr Scene();

protected:
	Renderer(const Viewport_ptr& viewport);

	virtual ~Renderer();

	void Create2DOverlayScene();

	bool				m_ShowDebugElements;

	Scene_ptr			m_Scene;
	Scene_ptr			overlay2Dscene;
	GeometryBuffer_ptr	m_gBuffer;
	Viewport_ptr		m_Viewport;

};

