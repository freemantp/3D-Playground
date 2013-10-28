#pragma once

#include "PhongShader.h"

SHARED_PTR_CLASS_DECL(Texture)
SHARED_PTR_CLASS_DECL(PhongTextureShader)

class PhongTextureShader :public PhongShader
{
public:

	static PhongTextureShader_ptr Create(const string& textureFile);
	
	virtual ~PhongTextureShader();

	virtual void use(const Scene_ptr scene, const glm::mat4& modelTransform);
	virtual void setSpecularMap(const std::string& specularMap);
	virtual void setBumpMap(const std::string& textureFile, bool isNormalMap);


protected:

	PhongTextureShader(const string& textureFile);

	enum TextureType {Albedo, BumpMap, Specular};
	static const int numTextures = 3;

	GLuint texUnits[numTextures];
	Texture_ptr textures[numTextures];
	bool isNormalMap;
	bool hasBumpMap;
	bool hasSpecularMap;

};

