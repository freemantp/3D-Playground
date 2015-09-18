#pragma once

#include "PhongShader.h"

#include <array>

SHARED_PTR_CLASS_DECL(Texture2D)
SHARED_PTR_CLASS_DECL(PhongTextureShader)
SHARED_PTR_CLASS_DECL(TextureMaterial)

class PhongTextureShader : public PhongShader
{
public:

	SHARED_PTR_FACTORY(PhongTextureShader);

	virtual bool Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	void UnUse() override;

	virtual bool SetMaterial(Material_cptr material) override;	

	VertexAttributeInfo_ptr GetVertexAttributeInfo() const override;

protected:

	PhongTextureShader();

	virtual ~PhongTextureShader();
	
	enum TextureType { Albedo = 0, BumpMap = 1, Specular = 2, Evironment = 3 };
	static const int numTextures = 4;

	std::array<GLuint, numTextures> textureUnits;

	TextureMaterial_cptr textureMaterial;

};

