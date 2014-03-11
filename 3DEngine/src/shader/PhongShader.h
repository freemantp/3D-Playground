#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"
#include "../util/gettersetter.h"

SHARED_PTR_CLASS_DECL(PhongShader)
SHARED_PTR_CLASS_DECL(PhongMaterial)
SHARED_PTR_CLASS_DECL(Shadow)

class PhongShader : public MaterialShader
	, public std::enable_shared_from_this<PhongShader>
{
public:

	static PhongShader_ptr Create();

	virtual ~PhongShader();

	virtual bool Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(Material_cptr material) override;

	virtual void UnUse() override;

	void SetShadow(Shadow_ptr);

protected:

	PhongShader();
	PhongShader(const string& shaderName);

	void SetLightAndModel(const Scene_ptr scene);


	virtual void Init();

	PhongMaterial_cptr material;
	Shadow_ptr shadow;

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;

};

