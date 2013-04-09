#pragma once
#pragma once;

#include <vector>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Camera);
SHARED_PTR_CLASS_DECL(PointLight);
SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(LightModel);
SHARED_PTR_CLASS_DECL(UniformBuffer);
SHARED_PTR_CLASS_DECL(GLSLProgram);

using std::vector;

typedef vector<PointLight*>::const_iterator PointLightIt;

class LightModel
{
public:
	LightModel();

	~LightModel();

	vector<PointLight_ptr> pointLights;
	vector<SpotLight_ptr> spotLights;

	UniformBuffer_ptr getLightsBuffer() const;
	void updateUniformBuffer(Camera_cptr cam);

protected:
	
	UniformBuffer_ptr lightsBuffer;

};

