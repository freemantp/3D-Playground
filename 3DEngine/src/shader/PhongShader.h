#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"
#include "../util/gettersetter.h"

SHARED_PTR_CLASS_DECL(PhongShader)
SHARED_PTR_CLASS_DECL(PhongMaterial)

class PhongShader 
	: public MaterialShader
	, public std::enable_shared_from_this<PhongShader>
{
public:

	static PhongShader_ptr Create();

	virtual bool Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(Material_cptr material) override;

	virtual void UnUse() override;

protected:

	PhongShader();
	PhongShader(const std::string& shaderName);

	virtual ~PhongShader();

	void SetLightAndModel(const Scene_ptr scene, const unsigned int tex_unit_offset = 0);

	virtual void Init();

	PhongMaterial_cptr material;

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;
	bool hasShadows;
	bool useShadows;
	bool pcfShadows;

};

