#include "stdafx.h"
#include "Material.h"

#include "../shader/ShaderLibrary.h"

#define SHARED_MATERIAL_PTR_FACTORY_IMPL(T) \
std::shared_ptr<T> T::Create() \
{ \
	std::shared_ptr<T> ptr = std::shared_ptr<T>(new T()); \
	if (auto sl = ShaderLibrary::GetInstance()) \
		sl->AddShader(ptr); \
	return ptr; \
}

SHARED_MATERIAL_PTR_FACTORY_IMPL(PhongMaterial);
SHARED_MATERIAL_PTR_FACTORY_IMPL(TextureMaterial);
SHARED_MATERIAL_PTR_FACTORY_IMPL(IntrinsicColorMaterial);
SHARED_MATERIAL_PTR_FACTORY_IMPL(ConstantColorMaterial);
SHARED_MATERIAL_PTR_FACTORY_IMPL(ShDiffuseMaterial);
SHARED_MATERIAL_PTR_FACTORY_IMPL(SkyboxMaterial);

PhongMaterial::PhongMaterial()
: shininess(1)
, opacity(1)
{

}