#pragma once

#include "ShaderBase.h"
#include "../light/PointLight.h"

class DiffusePerVertexShader :
	public ShaderBase
{
public:
	DiffusePerVertexShader();
	virtual ~DiffusePerVertexShader();
	virtual void use(const Camera& cam, const glm::mat4& modelTransform);

	PointLight& getLight();
	void setLight(PointLight lightSource);

private:
	PointLight lightSource;
	glm::vec3 materialColor;

};

