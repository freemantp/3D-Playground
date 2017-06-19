#pragma once

#include <glm/glm.hpp>
#include "../util/SharedPointer.h"
#include "../rendering/Renderable.h"

#include "../math/BoundingBox.h"


//forward declarations
class Scene;

SHARED_PTR_CLASS_DECL(Shape);
SHARED_PTR_CLASS_DECL(Material);

class Shape : public Renderable
{
public:
	virtual ~Shape();

	virtual void Init() = 0;

	void SetMaterial(const Material_ptr& material) { this->material = material; };

	Material_ptr GetMaterial() const { return material; };

	virtual void SetWorldTransform(const glm::mat4& t);
	const glm::mat4& WorldTransform() const;

	/// Returns the bounding box in world coordinates
	virtual AABBox BoundingBox() const;
	
protected:

	Shape();

	Shape(const Shape& rhs);

	Material_ptr material;
	AABBox bboxModelSpace;
	glm::mat4 worldTransform;

private:	
	mutable bool boundingBoxDirty;
	mutable AABBox cachedWorldBBox;
};

