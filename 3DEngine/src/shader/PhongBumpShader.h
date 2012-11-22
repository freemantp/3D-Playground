#pragma once

#include "PhongShader.h"

class Texture;

class PhongBumpShader :
	public PhongShader
{
public:
	PhongBumpShader(const string& textureFile, const string& normalMap);
	virtual ~PhongBumpShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);


protected:

	enum TextureType {Albedo, NormalMap};
	static const int numTextures = 2;

	GLuint texUnits[numTextures];
	Texture* textures[numTextures];

};

