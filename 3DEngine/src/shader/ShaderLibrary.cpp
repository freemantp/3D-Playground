#include "stdafx.h"
#include "ShaderLibrary.h"
#include "PhongShader.h"
#include "PhongTextureShader.h"
#include "IntrinsicColorShader.h"
#include "ConstColorShader.h"
#include "SHDiffuseShader.h"
#include "SkyboxShader.h"

// static initialisation
ShaderLibrary_ptr ShaderLibrary::instance = ShaderLibrary_ptr(new ShaderLibrary());

ShaderLibrary::ShaderLibrary()
{
}


ShaderLibrary::~ShaderLibrary()
{
}

ShaderLibrary_ptr ShaderLibrary::GetInstance()
{
	return instance;
}

bool ShaderLibrary::AddShader(Material_cptr material)
{
	if (!GetShader(material))
	{
		if (std::dynamic_pointer_cast<const TextureMaterial>(material))
			return AddShader(material, PhongTextureShader::Create());
		else if (std::dynamic_pointer_cast<const PhongMaterial>(material))
			return AddShader(material, PhongShader::Create());
		else if (std::dynamic_pointer_cast<const IntrinsicColorMaterial>(material))
			return AddShader(material, IntrinsicColorShader::Create());
		else if (std::dynamic_pointer_cast<const ConstantColorMaterial>(material))
			return AddShader(material, ConstColorShader::Create());
		else if (std::dynamic_pointer_cast<const ShDiffuseMaterial>(material))
			return AddShader(material, ShDiffuseShader::Create());
		else if (std::dynamic_pointer_cast<const SkyboxMaterial>(material))
			return AddShader(material, SkyboxShader::Create());
	}
	return false;
}

bool ShaderLibrary::AddShader(Material_cptr material, MaterialShader_ptr shader)
{
	const std::type_info& info = typeid(*material.get());
	return AddShader(info, shader);
}

bool ShaderLibrary::AddShader(const std::type_info& materialType, MaterialShader_ptr shader)
{
	bool success = false;

	if (materialShaderDictionary.count(&materialType) == 0)
	{
		materialShaderDictionary[&materialType] = shader;
		success = true;
	}
	return success;
}

MaterialShader_ptr ShaderLibrary::GetShader(Material_cptr material)
{
	const std::type_info& info = typeid(*material.get());
	return GetShader(info);
}

MaterialShader_ptr ShaderLibrary::GetShader(const std::type_info& materialType)
{
	if (materialShaderDictionary.count(&materialType) > 0)
		return materialShaderDictionary[&materialType];
	else
		return MaterialShader_ptr(); //TODO: create shader with factory
}