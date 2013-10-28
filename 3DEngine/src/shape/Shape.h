#pragma once

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"
#include "../rendering/Renderable.h"


//forward declarations
class Scene;

SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(ShaderBase);

class Shape : public Renderable
{
public:

	Shape();
	virtual ~Shape();

	virtual void init() = 0;
	
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

