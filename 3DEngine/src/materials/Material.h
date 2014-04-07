#pragma once

#include "../util/SharedPointer.h"
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(Material);
SHARED_PTR_CLASS_DECL(IntrinsicColorMaterial);
SHARED_PTR_CLASS_DECL(ConstantColorMaterial);
SHARED_PTR_CLASS_DECL(PhongMaterial);
SHARED_PTR_CLASS_DECL(TextureMaterial);
SHARED_PTR_CLASS_DECL(Texture2D);
SHARED_PTR_CLASS_DECL(ShDiffuseMaterial);
SHARED_PTR_CLASS_DECL(ShDiffuseShaderCoeffs);
SHARED_PTR_CLASS_DECL(SkyboxMaterial);
SHARED_PTR_CLASS_DECL(CubeMapTexture);

class Material
{
public:
	virtual ~Material() {};

	void SetName(const std::string& name);
	std::string GetName();

protected:
	Material() {};
	std::string name;
	
};

class ConstantColorMaterial : public Material
{
public:
	static ConstantColorMaterial_ptr Create();

	glm::vec3 color;
};

class PhongMaterial : public Material
{
public:
	static PhongMaterial_ptr Create();

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
	static TextureMaterial_ptr Create();
	Texture2D_ptr albedoTexture;
	Texture2D_ptr bumpTexture;
	Texture2D_ptr specularTexture;
	bool bumpIsNormalMap;

protected:
	TextureMaterial() { };
};

class IntrinsicColorMaterial : public Material
{
public:
	static IntrinsicColorMaterial_ptr Create();
};

class ShDiffuseMaterial : public Material
{
public:
	static ShDiffuseMaterial_ptr Create();

	ShDiffuseShaderCoeffs_ptr shCoeffs;

protected:
	ShDiffuseMaterial() {};
};

class SkyboxMaterial : public Material
{
public:
	static SkyboxMaterial_ptr Create();

	CubeMapTexture_ptr texture;
protected:
	SkyboxMaterial() {};
};

