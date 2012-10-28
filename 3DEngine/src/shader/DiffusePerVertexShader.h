#pragma once
#include "ShaderBase.h"

class DiffusePerVertexShader :
	public ShaderBase
{
public:
	DiffusePerVertexShader(const Camera& cam);
	virtual ~DiffusePerVertexShader(void);

	void setLightPosition(const glm::vec4& position);

private:
	const Camera& cam;
	

};

