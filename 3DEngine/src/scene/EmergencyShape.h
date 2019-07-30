#pragma once

#include "../shape/Shape.h"
#include "../core/gl.h"

class EmergencyShape: public Shape
{
public:

	// Inherited via Shape
	virtual void Render(const Scene_ptr& scene) const override;

	virtual void RenderShadowMap(const ShadowMapShader_ptr&) const override;

	virtual void Init() override;

private:

	//GLuint shader_programme;
	GLuint vao;

};

