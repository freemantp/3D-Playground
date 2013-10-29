#pragma once

#include "ShaderBase.h"
#include "../util/gettersetter.h"

SHARED_PTR_CLASS_DECL(ShDiffuseShader)

class ShDiffuseShader 
	: public ShaderBase
	, public std::enable_shared_from_this<ShDiffuseShader>
{
public:

	static ShDiffuseShader_ptr Create();

	virtual ~ShDiffuseShader();

	virtual void use(const Scene_ptr scene, const glm::mat4& modelTransform);

protected:

	ShDiffuseShader();
	ShDiffuseShader(const string& shaderName);

	virtual void init();

};

