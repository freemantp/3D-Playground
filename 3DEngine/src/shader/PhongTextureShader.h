#pragma once

#include "PhongShader.h"

SHARED_PTR_CLASS_DECL(Texture)
SHARED_PTR_CLASS_DECL(PhongTextureShader)

class PhongTextureShader :public PhongShader
{
public:

	static PhongTextureShader_ptr Create(const Texture_ptr albedoTex);
	
	virtual ~PhongTextureShader();

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform);
	virtual void SetSpecularMap(const Texture_ptr specularMa);
	virtual void SetBumpMap(const Texture_ptr bumpMap, bool isNormalMap);


protected:

	PhongTextureShader(const Texture_ptr albedo);

	enum TextureType {Albedo, BumpMap, Specular};
	static const int numTextures = 3;

	GLuint texUnits[numTextures];
	Texture_ptr textures[numTextures];
	bool isNormalMap;
	bool hasBumpMap;
	bool hasSpecularMap;

};

