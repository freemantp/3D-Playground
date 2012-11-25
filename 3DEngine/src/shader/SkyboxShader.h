#pragma once
#include "shaderbase.h"

class CubeMapTexture;

class SkyboxShader :
	public ShaderBase
{
public:
	SkyboxShader(CubeMapTexture* texture);
	~SkyboxShader(void);

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

protected:
	GLuint texUnit;
	CubeMapTexture* texture;
};

