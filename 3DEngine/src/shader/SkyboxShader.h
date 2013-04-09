#pragma once

#include "shaderbase.h"
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(SkyboxShader);
SHARED_PTR_CLASS_DECL(CubeMapTexture);

class SkyboxShader : public ShaderBase
{
public:
	SkyboxShader(CubeMapTexture_ptr texture);
	~SkyboxShader(void);

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

protected:
	GLuint texUnit;
	CubeMapTexture_ptr texture;
};

