#include "stdafx.h"
#include "Renderer.h"
#include "Viewport.h"
#include "GeometryBuffer.h"
#include  "../scene/Scene.h"


Renderer_ptr Renderer::Create(Viewport_ptr viewport)
{
	return Renderer_ptr(new Renderer(viewport));
}

Renderer::Renderer(Viewport_ptr viewport)
	: m_Viewport(viewport)
{
}

Renderer::~Renderer()
{
}

void Renderer::SetScene(Scene_ptr scene)
{
	m_Scene = scene;
}
Scene_ptr Renderer::Scene()
{
	return m_Scene;
}

void Renderer::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(m_Scene)
	{
		m_Scene->Render(m_Viewport);
	}
}

void Renderer::ViewportChanged(Viewport_ptr viewport)
{
	m_Viewport = viewport;
}
