#pragma once

#include "PhongShader.h"

class Texture;

class PhongBumpShader :
	public PhongShader
{
public:
	PhongBumpShader(const string& textureFile, const std::string& normalMap, bool isNormalMap);
	virtual ~PhongBumpShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);
	virtual void setSpecularMap(const std::string& specularMap);


protected:

	enum TextureType {Albedo, BumpMap, Specular};
	static const int numTextures = 3;

	GLuint texUnits[numTextures];
	Texture* textures[numTextures];
	bool isNormalMap;
	bool hasSpecularMap;

};

