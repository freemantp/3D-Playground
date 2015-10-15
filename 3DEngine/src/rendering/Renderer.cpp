#include "stdafx.h"
#include "Renderer.h"
#include "Viewport.h"
#include "GeometryBuffer.h"

#include  "../scene/Scene.h"
#include  "../scene/DebugScene.h"

#include  "../util/TimeManager.h"

#include "../error.h"

Renderer_ptr Renderer::Create(const Viewport_ptr& viewport)
{
	return Renderer_ptr(new Renderer(viewport), [](Renderer* r) {delete r; });
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
	CreateDebugScene();
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

	if (m_ShowDebugElements && debugScene)
	{
		glDisable(GL_DEPTH_TEST);
		debugScene->Render(m_Viewport);
		glEnable(GL_DEPTH_TEST);
	}
}

void Renderer::ViewportChanged(const Viewport_ptr& viewport)
{
	m_Viewport = viewport;
}

void Renderer::OnKey(const Input::Key key, const Input::Modifier mod, const glm::vec2& position)
{
	if (key == Input::Key::D)
	{
		auto& tm = TimeManager::GetInstance();

		if (!m_ShowDebugElements)			
			tm.AddTimeObserver(debugScene);
		else
			tm.RemoveTimeObserver(debugScene);

		m_ShowDebugElements = !m_ShowDebugElements;

		std::string msg = (m_ShowDebugElements ? "Enabling" : "Disabling");
		msg += " debug mode";
		Debug(msg);
	}
}

void Renderer::CreateDebugScene()
{
	if (m_Scene)
	{
		debugScene = DebugScene::Create(m_Scene);
	}
}
