#pragma once
#include "ShaderBase.h"

class DiffusePerVertexShader :
	public ShaderBase
{
public:
	DiffusePerVertexShader(const Camera& cam);
	virtual ~DiffusePerVertexShader();

	void setLightPosition(const glm::vec4& position);

	glm::vec4 getLightPosition();

private:

	glm::vec4 lightPosition;

};

