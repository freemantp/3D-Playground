#pragma once

#include "ShaderBase.h"
#include "../util/gettersetter.h"

namespace GLSLShader 
{
	enum PhongShadeModel 
	{
		PHONG, BLINN_PHONG
	};
};

SHARED_PTR_CLASS_DECL(PhongShader)

class PhongShader 
	: public ShaderBase
	, public std::enable_shared_from_this<PhongShader>
{
public:

	static PhongShader_ptr Create();

	virtual ~PhongShader();

	virtual void use(const Scene_ptr scene, const glm::mat4& modelTransform);

	vec3 ambientReflection;
	vec3 diffuseReflection;
	vec3 glossyReflection;

	int shininess;
	GLSLShader::PhongShadeModel shadeModel;

protected:

	PhongShader();
	PhongShader(const string& shaderName);

	void setLightAndModel(const Scene_ptr scene);

	virtual void init();

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;

};

