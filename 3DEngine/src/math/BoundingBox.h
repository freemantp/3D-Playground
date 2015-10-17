#pragma once;

#define GLM_SWIZZLE
#include <glm/glm.hpp>

class AABBox
{
public:

	AABBox();

	AABBox(const glm::vec3& lower, const glm::vec3& upper);

	virtual glm::vec3 LowerLeft() const;

	virtual glm::vec3 UpperRight() const;

	/// Transforms the bbox and returns it; the current instance remains untouched
	virtual AABBox Transform(const glm::mat4& t) const;

	/// Creates the hull bounding box of this box an the passed box
	virtual AABBox HullBox(const AABBox& other) const;

	inline void operator*=(const glm::mat4& rhs);
	
	inline void operator+=(const AABBox& rhs) { *this = HullBox(rhs); };

	inline AABBox operator+(const AABBox& rhs) { return HullBox(rhs); }

	glm::vec3 d;
	glm::vec3 p;
	glm::mat3 R;
};

inline AABBox operator*(const glm::mat4& lhs, const AABBox& rhs) {
	return rhs.Transform(lhs);
}

/// Oriented bounding box
class OBBox : public AABBox
{
public:

	OBBox();

	OBBox(const glm::vec3& lower, const glm::vec3& upper);

	virtual glm::vec3 LowerLeft() const override;

	virtual glm::vec3 UpperRight() const override;

	glm::vec3 d;
	glm::vec3 p;
	glm::mat3 R;
};