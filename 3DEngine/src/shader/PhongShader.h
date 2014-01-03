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

	virtual bool SetMaterial(Material_ptr) override;

	virtual void UnUse() override;

	//vec3 ambientReflection;
	//vec3 diffuseReflection;
	//vec3 glossyReflection;

	//int shininess;
	//float opacity;
	GLSLShader::PhongShadeModel shadeModel;

protected:

	PhongShader();
	PhongShader(const string& shaderName);

	void SetLightAndModel(const Scene_ptr scene);

	virtual void Init();

	PhongMaterial_ptr material;

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;

};

