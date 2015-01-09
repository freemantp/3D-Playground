#include "stdafx.h"
#include "Renderer.h"
#include "Viewport.h"
#include "GeometryBuffer.h"
#include  "../scene/Scene.h"

#include "../camera/OrthogonalCamera.h"


Renderer_ptr Renderer::Create(Viewport_ptr viewport)
{
	return Renderer_ptr(new Renderer(viewport), [](Renderer* r) {delete r; });
}

Renderer::Renderer(Viewport_ptr viewport)
	: m_Viewport(viewport)
	, m_ShowDebugElements(true)
{
}

Renderer::~Renderer()
{
}

void Renderer::SetScene(Scene_ptr scene)
{
	if (scene)
	{
		m_Scene = scene;

		auto ortho_cam = std::shared_ptr<OrthogonalCamera>(new OrthogonalCamera(10));
		m_DebugScene = Scene::Create(ortho_cam);
	}
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

	if (m_ShowDebugElements && m_DebugScene)
	{
		m_DebugScene->Render(m_Viewport);
	}
}

void Renderer::ViewportChanged(Viewport_ptr viewport)
{
	m_Viewport = viewport;
}
