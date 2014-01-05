#include "stdafx.h"
#include "ShaderLibrary.h"

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