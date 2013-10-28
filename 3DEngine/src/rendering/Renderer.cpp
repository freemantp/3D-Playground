#include "stdafx.h"
#include "Renderer.h"
#include  "../scene/Scene.h"

#include <GL/glew.h>

Renderer_ptr Renderer::Create()
{
	return Renderer_ptr(new Renderer());
}

Renderer::Renderer()
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
		m_Scene->render();
}
