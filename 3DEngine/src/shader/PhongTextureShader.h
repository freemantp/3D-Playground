#pragma once

#include "PhongShader.h"

SHARED_PTR_CLASS_DECL(Texture2D)
SHARED_PTR_CLASS_DECL(PhongTextureShader)
SHARED_PTR_CLASS_DECL(TextureMaterial)

class PhongTextureShader : public PhongShader
{
public:

	SHARED_PTR_FACTORY(PhongTextureShader);
	
	virtual ~PhongTextureShader();

	virtual bool Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(Material_cptr material) override;	

protected:

	PhongTextureShader();

	enum TextureType { Albedo = 0, BumpMap = 1, Specular = 2 };
	static const int numTextures = 3;

	GLuint texUnits[numTextures];
	TextureMaterial_cptr textureMaterial;

};

