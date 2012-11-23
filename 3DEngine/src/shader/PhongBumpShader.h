#pragma once

#include "PhongShader.h"

class Texture;

class PhongBumpShader :
	public PhongShader
{
public:
	PhongBumpShader(const string& textureFile, const string& normalMap, bool isNormalMap);
	virtual ~PhongBumpShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);


protected:

	enum TextureType {Albedo, BumpMap};
	static const int numTextures = 2;

	GLuint texUnits[numTextures];
	Texture* textures[numTextures];
	bool isNormalMap;

};

