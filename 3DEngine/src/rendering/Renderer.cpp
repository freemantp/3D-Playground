#include "stdafx.h"
#include "Renderer.h"
#include "GeometryBuffer.h"
#include  "../scene/Scene.h"

#include <GL/glew.h>

Renderer_ptr Renderer::Create(int width, int height)
{
	return Renderer_ptr(new Renderer(width,height));
}

Renderer::Renderer(int width, int height)
	: m_gBuffer(GeometryBuffer::Create(width,height))
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
		//m_gBuffer->StartGBufferComp(m_Scene,
		m_Scene->render();
	}
}
