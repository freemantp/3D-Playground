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

ShaderLibrary_cptr ShaderLibrary::GetInstance()
{
	return instance;
}

bool ShaderLibrary::AddShader(Material_cptr material, ShaderBase_ptr shader)
{
	const std::type_info& info = typeid(*material.get());
	return AddShader(info, shader);
}

bool ShaderLibrary::AddShader(const std::type_info& materialType, ShaderBase_ptr shader)
{
	bool success = false;

	if (materialShaderDictionary.count(&materialType) == 0)
	{
		materialShaderDictionary[&materialType] = shader;
		success = true;
	}
	return success;
}

ShaderBase_ptr ShaderLibrary::GetShader(Material_cptr material)
{
	const std::type_info& info = typeid(*material.get());
	return GetShader(info);
}

ShaderBase_ptr ShaderLibrary::GetShader(const std::type_info& materialType)
{
	if (materialShaderDictionary.count(&materialType) > 0)
		return materialShaderDictionary[&materialType];
	else
		return ShaderBase_ptr(); //TODO: create shader with factory
}