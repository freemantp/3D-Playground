#pragma once

#include "PhongShader.h"

class Texture;

class PhongTextureShader :
	public PhongShader
{
public:
	PhongTextureShader(const string& textureFile);
	virtual ~PhongTextureShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);


protected:

	enum TextureType {Albedo, Other};
	static const int numTextures = 2;

	GLuint texUnits[numTextures];
	Texture* textures[numTextures];

};

