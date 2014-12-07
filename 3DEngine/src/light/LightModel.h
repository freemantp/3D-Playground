#pragma once;

#include <vector>

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(PointLight);
SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(DirectionalLight);
SHARED_PTR_CLASS_DECL(LightModel);
SHARED_PTR_CLASS_DECL(UniformBuffer);
SHARED_PTR_CLASS_DECL(GLSLProgram);
SHARED_PTR_CLASS_DECL(Texture3D);

class LightModel
{
public:
	LightModel();

	~LightModel();

	UniformBuffer_ptr GetLightsBuffer() const;
	void UpdateUniformBuffer(Camera_cptr cam);

	bool IsValid();

public:

	std::vector<PointLight_ptr> pointLights;
	std::vector<SpotLight_ptr> spotLights;
	DirectionalLight_ptr directionalLight;

	Texture3D_ptr pcfShadowRandomData;

protected:

	UniformBuffer_ptr lightsBuffer;
	bool valid;
};

