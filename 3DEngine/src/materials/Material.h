#pragma once

#include "../util/SharedPointer.h"
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(Material);
SHARED_PTR_CLASS_DECL(IntrinsicColorMaterial);
SHARED_PTR_CLASS_DECL(ConstantColorMaterial);
SHARED_PTR_CLASS_DECL(PhongMaterial);
SHARED_PTR_CLASS_DECL(TextureMaterial);
SHARED_PTR_CLASS_DECL(Texture);

class Material
{
public:
	virtual ~Material() {};
protected:
	Material() {};	
	
};

class IntrinsicColorMaterial : public Material
{
public:
	static IntrinsicColorMaterial_ptr Create();
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
	static TextureMaterial_ptr Create(Texture_ptr albedoTexture);
	Texture_ptr albedoTexture;
	Texture_ptr bumpTexture;
	Texture_ptr specularTexture;
	bool bumpIsNormalMap;

protected:
	TextureMaterial(Texture_ptr albedo);
};