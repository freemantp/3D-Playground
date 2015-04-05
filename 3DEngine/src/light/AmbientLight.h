#pragma once

#include "Light.h"

#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(AmbientLight);


class AmbientLight : public Light
{
public:
	
	SHARED_PTR_FACTORY(AmbientLight)

protected:
	AmbientLight();

	virtual ~AmbientLight();

};

