#pragma once
#include "Box.h"
#include <string>

SHARED_PTR_CLASS_DECL(Skybox);
SHARED_PTR_CLASS_DECL(SkyboxMaterial);

class Skybox : public Box
{
public:

	Skybox(const SkyboxMaterial_ptr& material);
	virtual ~Skybox();

};

