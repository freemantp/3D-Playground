#pragma once;

#include <vector>

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(PointLight);
SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(LightModel);
SHARED_PTR_CLASS_DECL(UniformBuffer);
SHARED_PTR_CLASS_DECL(GLSLProgram);
SHARED_PTR_CLASS_DECL(Texture3D);

using std::vector;

typedef vector<PointLight*>::const_iterator PointLightIt;

class LightModel
{
public:
	LightModel();

	~LightModel();

	vector<PointLight_ptr> pointLights;
	vector<SpotLight_ptr> spotLights;

	Texture3D_ptr pcfShadowRandomData;

	UniformBuffer_ptr GetLightsBuffer() const;
	void UpdateUniformBuffer(Camera_cptr cam);

	bool IsValid();

protected:

	UniformBuffer_ptr lightsBuffer;
	bool valid;
};

