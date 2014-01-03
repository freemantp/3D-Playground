#pragma once

#include "PhongShader.h"

#include <cstdint>

SHARED_PTR_CLASS_DECL(Texture)
SHARED_PTR_CLASS_DECL(PhongTextureShader)
SHARED_PTR_CLASS_DECL(TextureMaterial)

class PhongTextureShader : public PhongShader
{
public:

	static PhongTextureShader_ptr Create(const Texture_ptr albedoTex);
	
	virtual ~PhongTextureShader();

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual void SetAlbedo(const Texture_ptr albedoTex);
	virtual void SetSpecularMap(const Texture_ptr specularMap);
	virtual void SetBumpMap(const Texture_ptr bumpMap, bool isNormalMap);

protected:

	PhongTextureShader(const Texture_ptr albedo);

	enum TextureType { Albedo = 0, BumpMap = 1, Specular = 2 };
	static const int numTextures = 3;

	GLuint texUnits[numTextures];
	Texture_ptr textures[numTextures];
	bool isNormalMap;
	bool hasBumpMap;
	bool hasSpecularMap;

};

