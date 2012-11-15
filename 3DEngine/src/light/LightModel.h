#pragma once
#pragma once;

#include <vector>

class PointLight;

using std::vector;

typedef vector<PointLight*>::const_iterator PointLightIt;

class LightModel
{
public:
	LightModel(void);
	~LightModel(void);

	vector<PointLight*> pointLights;

protected:

	void updatePLUniformBuffer();

	

};

