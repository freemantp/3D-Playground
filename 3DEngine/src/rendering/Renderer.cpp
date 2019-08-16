#include "stdafx.h"
#include "Renderer.h"
#include "Viewport.h"
#include "GeometryBuffer.h"

#include  "../scene/Scene.h"
#include  "../scene/SceneOverlay2D.h"

#include  "../util/TimeManager.h"

#include "../error.h"

Renderer_ptr Renderer::Create(const Viewport_ptr& viewport)
{
	return std::make_shared<Renderer>(viewport);
}

Renderer::Renderer(const Viewport_ptr& viewport)
	: m_Viewport(viewport)
	, m_ShowDebugElements(false)
{
}

Renderer::~Renderer()
{
}

void Renderer::SetScene(Scene_ptr scene)
{
	m_Scene = scene;
	Create2DOverlayScene();
}
Scene_ptr Renderer::Scene()
{
	return m_Scene;
}

void Renderer::Render()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_Scene)
	{
		m_Scene->Render(m_Viewport);
	}

	if (m_ShowDebugElements && overlay2Dscene)
	{
		glDisable(GL_DEPTH_TEST);
		overlay2Dscene->Render(m_Viewport);
		glEnable(GL_DEPTH_TEST);
	}
}

void Renderer::ViewportChanged(const Viewport_ptr& viewport)
{
	m_Viewport = viewport;
}

void Renderer::OnKey(const Input::Key key, const Input::Modifier mod)
{
	if (key == Input::Key::D)
	{
		auto& tm = TimeManager::Instance();

		if (!m_ShowDebugElements)			
			tm.AddTimeObserver(overlay2Dscene);
		else
			tm.RemoveTimeObserver(overlay2Dscene);

		m_ShowDebugElements = !m_ShowDebugElements;

		std::string msg = (m_ShowDebugElements ? "Enabling" : "Disabling");
		msg += " debug mode";
		Debug(msg);
	}
	else if (key == Input::Key::B)
	{
		if (m_Scene)
			m_Scene->SetRenderBoundingBoxes(!m_Scene->RenderBoundingBoxes());
	}
	else if (key == Input::Key::L)
	{
		if (m_Scene)
			m_Scene->SetRenderLightRepresentation(!m_Scene->RenderLightRepresentation());
	}
}

void Renderer::Create2DOverlayScene()
{
	if (m_Scene)
	{
		overlay2Dscene = SceneOverlay2D::Create(m_Scene);
	}
}
