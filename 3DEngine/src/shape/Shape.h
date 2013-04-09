#pragma once

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"


//forward declarations
class Scene;

SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(ShaderBase);

class Shape
{
public:

	Shape();
	virtual ~Shape();

	virtual void init() = 0;
	virtual void render(const Scene& scene) const = 0;
	virtual void setShader(ShaderBase_ptr shader);

	ShaderBase_ptr getShader()
	{
		return shaderProgram;
	}

	void setWorldTransform(glm::mat4 transf)
	{
		worldTransform = transf;
	}
	
	glm::mat4 worldTransform;

protected:
	ShaderBase_ptr shaderProgram;
};

