#pragma once

#include "ShaderBase.h"
#include "../util/gettersetter.h"

SHARED_PTR_CLASS_DECL(ShDiffuseShader)
SHARED_PTR_CLASS_DECL(ShDiffuseShaderCoeffs)

class ShDiffuseShaderCoeffs 
{
public:

	static ShDiffuseShaderCoeffs_ptr Create()
	{
		return ShDiffuseShaderCoeffs_ptr(new ShDiffuseShaderCoeffs());
	}

	std::string name;
	std::vector<glm::vec3> m_Coeffs;

protected:
	ShDiffuseShaderCoeffs() {};

};

class ShDiffuseShader : public ShaderBase
{
public:

	static ShDiffuseShader_ptr Create();

	virtual ~ShDiffuseShader();

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform);

protected:

	ShDiffuseShader();

	virtual void Init();

	ShDiffuseShaderCoeffs_ptr m_ShCoeffs;

};

