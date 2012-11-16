#pragma once
#pragma once;

#include <vector>

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
	UniformBuffer* getLightsBuffer() const;
	void updateUniformBuffer(const Camera* cam);

protected:

	
	UniformBuffer* lightsBuffer;

};

