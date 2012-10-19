#pragma once

#include "Matrix4x4.h"
#include "Vector.h"
#include "Point.h"

// Transform Declarations
class Transform {
public:
    // Transform Public Methods
    Transform(void);
    Transform(const float mat[4][4]);
    Transform(const Matrix4x4 &mat);
    Transform(const Matrix4x4 &mat, const Matrix4x4 &minv);

	~Transform(void);

    void Print(FILE *f) const;
    friend Transform Inverse(const Transform &t);
    friend Transform Transpose(const Transform &t);
    bool operator==(const Transform &t) const;
    bool operator!=(const Transform &t) const ;
    bool operator<(const Transform &t2) const;
    bool IsIdentity() const;
    const Matrix4x4 &GetMatrix() const; 
    const Matrix4x4 &GetInverseMatrix() const;
    bool HasScale() const;
    inline Point operator()(const Point &pt) const;
    inline void operator()(const Point &pt, Point *ptrans) const;
    inline Vector operator()(const Vector &v) const;
    inline void operator()(const Vector &v, Vector *vt) const;

    Transform operator*(const Transform &t2) const;
    bool SwapsHandedness() const;

	static Transform Translate(const Vector &delta);
	static Transform Scale(float x, float y, float z);
	static Transform RotateX(float angle);
	static Transform RotateY(float angle);
	static Transform RotateZ(float angle);
	static Transform Rotate(float angle, const Vector &axis);
	static Transform LookAt(const Point &pos, const Point &look, const Vector &up);

private:
    // Transform Private Data
    Matrix4x4 m, mInv;
    friend class AnimatedTransform;
    friend struct Quaternion;
};



