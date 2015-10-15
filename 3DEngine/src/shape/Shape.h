#pragma once

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"
#include "../rendering/Renderable.h"

#include "../math/OBB.h"


//forward declarations
class Scene;

SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(Material);

class Shape : public Renderable
{
public:

	Shape();
	virtual ~Shape();

	virtual void Init() = 0;

	void SetMaterial(const Material_ptr& material) { this->material = material; };

	Material_ptr GetMaterial() const { return material; };
	
	glm::mat4 worldTransform;

	OBB boxModelSpace;

protected:
	Material_ptr material;
};

