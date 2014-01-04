#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"
#include "../util/gettersetter.h"

namespace GLSLShader 
{
	enum PhongShadeModel 
	{
		PHONG, BLINN_PHONG
	};
};

SHARED_PTR_CLASS_DECL(PhongShader)
SHARED_PTR_CLASS_DECL(PhongMaterial)

class PhongShader 
	: public ShaderBase
	, public MaterialShader
	, public std::enable_shared_from_this<PhongShader>
{
public:

	static PhongShader_ptr Create();

	virtual ~PhongShader();

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(Material_cptr material) override;

	virtual void UnUse() override;


	GLSLShader::PhongShadeModel shadeModel;

protected:

	PhongShader();
	PhongShader(const string& shaderName);

	void SetLightAndModel(const Scene_ptr scene);

	virtual void Init();

	PhongMaterial_cptr material;

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;

};

