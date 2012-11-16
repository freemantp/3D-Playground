#pragma once
#pragma once;

#include <vector>

class SpotLight;
class PointLight;
class UniformBuffer;
class GLSLProgram;
class Camera;

using std::vector;

typedef vector<PointLight*>::const_iterator PointLightIt;

class LightModel
{
public:
	LightModel();
	~LightModel();

	vector<PointLight*> pointLights;
	vector<SpotLight*> spotLights;

	UniformBuffer* getLightsBuffer() const;
	void updateUniformBuffer(const Camera* cam);

protected:

	
	UniformBuffer* lightsBuffer;

};

