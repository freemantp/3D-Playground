#pragma once

#include "PhongShader.h"

SHARED_PTR_CLASS_DECL(Texture)
SHARED_PTR_CLASS_DECL(PhongBumpShader)

class PhongBumpShader :public PhongShader
{
public:

	static PhongBumpShader_ptr Create(const string& textureFile, const std::string& normalMap, bool isNormalMap);
	
	virtual ~PhongBumpShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);
	virtual void setSpecularMap(const std::string& specularMap);


protected:

	PhongBumpShader(const string& textureFile, const std::string& normalMap, bool isNormalMap);

	enum TextureType {Albedo, BumpMap, Specular};
	static const int numTextures = 3;

	GLuint texUnits[numTextures];
	Texture_ptr textures[numTextures];
	bool isNormalMap;
	bool hasSpecularMap;

};

