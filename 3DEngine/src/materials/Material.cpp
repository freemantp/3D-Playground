#include "stdafx.h"
#include "Material.h"

IntrinsicColorMaterial_ptr IntrinsicColorMaterial::Create()
{
	return IntrinsicColorMaterial_ptr(new IntrinsicColorMaterial());
}

ConstantColorMaterial_ptr ConstantColorMaterial::Create()
{
	return ConstantColorMaterial_ptr(new ConstantColorMaterial());
}

PhongMaterial_ptr PhongMaterial::Create()
{
	return PhongMaterial_ptr(new PhongMaterial());
}

PhongMaterial::PhongMaterial()
: shininess(1)
, opacity(1)
{

}

TextureMaterial_ptr TextureMaterial::Create(Texture_ptr albedoTexture)
{
	return TextureMaterial_ptr(new TextureMaterial(albedoTexture));
}

TextureMaterial::TextureMaterial(Texture_ptr albedoTexture)
:  albedoTexture(albedoTexture)
{

}