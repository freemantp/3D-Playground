#pragma once

#include "ShaderBase.h"
#include "../light/PointLight.h"

class DiffusePerVertexShader :
	public ShaderBase
{
public:
	DiffusePerVertexShader(const Camera& cam);
	virtual ~DiffusePerVertexShader();
	virtual void use();

	PointLight& getLight();
	void setLight(PointLight lightSource);

private:
	PointLight lightSource;
	glm::vec3 materialColor;

};

