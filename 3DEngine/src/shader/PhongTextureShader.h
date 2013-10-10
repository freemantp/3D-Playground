#pragma once

#include "PhongShader.h"
#include "../util/gettersetter.h"

SHARED_PTR_CLASS_DECL(Texture)
SHARED_PTR_CLASS_DECL(PhongTextureShader)

class PhongTextureShader : public PhongShader
{
public:
	PhongTextureShader(const string& textureFile);
	virtual ~PhongTextureShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

	GETSET(float,EnvMapReflection)

protected:

	enum TextureType {Albedo, Environment};
	static const int numTextures = 2;

	GLuint texUnits[numTextures];
	Texture_ptr textures[numTextures];

	float envMapReflection;

};

