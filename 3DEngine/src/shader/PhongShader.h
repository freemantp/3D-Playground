#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(PhongShader)
SHARED_PTR_CLASS_DECL(PhongMaterial)

class PhongShader 
	: public MaterialShader
	, public std::enable_shared_from_this<PhongShader>
{
public:

	SHARED_PTR_FACTORY(PhongShader)

	PhongShader();

	virtual ~PhongShader();

	virtual bool Use(const Scene_ptr& scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(const Material_cptr& material) override;

	virtual void UnUse() override;

protected:

	PhongShader(const std::string& shaderName);

	void SetLightAndModel(const Scene_ptr& scene, const unsigned int tex_unit_offset = 0);

	virtual void Init();

	PhongMaterial_cptr material;

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;
	bool hasShadows;
	bool useShadows;
	bool pcfShadows;

};

