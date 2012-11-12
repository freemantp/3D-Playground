#pragma once

#include <glm/glm.hpp>

//forward declarations
class Scene;
class ShaderBase;

class Shape
{
public:

	Shape();
	virtual ~Shape();

	virtual void init() = 0;
	virtual void render(const Scene& scene) const = 0;
	virtual void setShader(ShaderBase* shader);

	ShaderBase* getShader()
	{
		return shaderProgram;
	}

	void setWorldTransform(glm::mat4 transf)
	{
		worldTransform = transf;
	}
	
	glm::mat4 worldTransform;

protected:
	ShaderBase* shaderProgram;
};

