#pragma once

#include "../util/SharedPointer.h"
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(Material);
SHARED_PTR_CLASS_DECL(IntrinsicColorMaterial);
SHARED_PTR_CLASS_DECL(ConstantColorMaterial);
SHARED_PTR_CLASS_DECL(PhongMaterial);
SHARED_PTR_CLASS_DECL(TextureMaterial);
SHARED_PTR_CLASS_DECL(Texture);
SHARED_PTR_CLASS_DECL(ShDiffuseMaterial);
SHARED_PTR_CLASS_DECL(ShDiffuseShaderCoeffs);
SHARED_PTR_CLASS_DECL(SkyboxMaterial);
SHARED_PTR_CLASS_DECL(CubeMapTexture);

class Material
{
public:
	virtual ~Material() {};
protected:
	Material() {};	
	
};

class ConstantColorMaterial : public Material
{
public:
	SHARED_PTR_FACTORY(ConstantColorMaterial);

	glm::vec3 color;
};

class PhongMaterial : public Material
{
public:
	SHARED_PTR_FACTORY(PhongMaterial);

	glm::vec3 ambientReflection;
	glm::vec3 diffuseReflection;
	glm::vec3 glossyReflection;
	int shininess;
	float opacity;
protected:
	PhongMaterial();
};

class TextureMaterial : public PhongMaterial
{
public:
	SHARED_PTR_FACTORY(TextureMaterial);
	Texture_ptr albedoTexture;
	Texture_ptr bumpTexture;
	Texture_ptr specularTexture;
	bool bumpIsNormalMap;

protected:
	TextureMaterial() { };
};

class IntrinsicColorMaterial : public Material
{
public:
	SHARED_PTR_FACTORY(IntrinsicColorMaterial);
};

class ShDiffuseMaterial : public Material
{
public:
	SHARED_PTR_FACTORY(ShDiffuseMaterial);

	ShDiffuseShaderCoeffs_ptr shCoeffs;

protected:
	ShDiffuseMaterial() {};
};

class SkyboxMaterial : public Material
{
public:
	SHARED_PTR_FACTORY(SkyboxMaterial);

	CubeMapTexture_ptr texture;
protected:
	SkyboxMaterial() {};
};

