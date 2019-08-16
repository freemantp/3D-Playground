#pragma once

#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(ShDiffuseShader)
SHARED_PTR_CLASS_DECL(ShDiffuseShaderCoeffs)
SHARED_PTR_CLASS_DECL(ShDiffuseMaterial)

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

class ShDiffuseShader : public MaterialShader
{
public:

	SHARED_PTR_FACTORY(ShDiffuseShader);

	virtual ~ShDiffuseShader();

	ShDiffuseShader();

	virtual bool Use(const Scene_ptr& scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(const Material_cptr& material) override;

	ShDiffuseMaterial_cptr material;

};

