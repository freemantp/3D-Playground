#include "stdafx.h"
#include "Transform.h"
#include <cassert>


Transform::Transform()
{
}


Transform::~Transform()
{
}


Transform::Transform(const float mat[4][4]) 
{
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                      mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                      mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                      mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = Inverse(m);
}

Transform::Transform(const Matrix4x4 &mat) 
	: m(mat), mInv(Inverse(mat)) {
}

Transform::Transform(const Matrix4x4 &mat, const Matrix4x4 &minv)
	: m(mat), mInv(minv) {
}

Transform Inverse(const Transform &t) 
{
    return Transform(t.mInv, t.m);
}

Transform Transpose(const Transform &t) 
{
    return Transform(Transpose(t.m), Transpose(t.mInv));
}

bool Transform::operator==(const Transform &t) const 
{
    return t.m == m && t.mInv == mInv;
}

bool Transform::operator!=(const Transform &t) const 
{
    return t.m != m || t.mInv != mInv;
}

bool Transform::operator<(const Transform &t2) const 
{
    for (uint32_t i = 0; i < 4; ++i)
        for (uint32_t j = 0; j < 4; ++j) {
            if (m.m[i][j] < t2.m.m[i][j]) return true;
            if (m.m[i][j] > t2.m.m[i][j]) return false;
        }
    return false;
}

bool Transform::IsIdentity() const 
{
    return (m.m[0][0] == 1.f && m.m[0][1] == 0.f &&
            m.m[0][2] == 0.f && m.m[0][3] == 0.f &&
            m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
            m.m[1][2] == 0.f && m.m[1][3] == 0.f &&
            m.m[2][0] == 0.f && m.m[2][1] == 0.f &&
            m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
            m.m[3][0] == 0.f && m.m[3][1] == 0.f &&
            m.m[3][2] == 0.f && m.m[3][3] == 1.f);
}

const Matrix4x4& Transform::GetMatrix() const 
{ 
	return m; 
}
const Matrix4x4& Transform::GetInverseMatrix() const 
{ 
	return mInv; 
}

bool Transform::HasScale() const 
{
    float la2 = (*this)(Vector(1,0,0)).LengthSquared();
    float lb2 = (*this)(Vector(0,1,0)).LengthSquared();
    float lc2 = (*this)(Vector(0,0,1)).LengthSquared();
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
    return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
}

	
// Transform Inline Functions
inline Point Transform::operator()(const Point &pt) const 
{
    float x = pt.x, y = pt.y, z = pt.z;
    float xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
    float yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
    float zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
    float wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
    assert(wp != 0);
    if (wp == 1.) return Point(xp, yp, zp);
    else          return Point(xp, yp, zp)/wp;
}


inline void Transform::operator()(const Point &pt,Point *ptrans) const 
{
    float x = pt.x, y = pt.y, z = pt.z;
    ptrans->x = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
    ptrans->y = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
    ptrans->z = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
    float w   = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
    if (w != 1.) *ptrans /= w;
}


inline Vector Transform::operator()(const Vector &v) const 
{
  float x = v.x, y = v.y, z = v.z;
  return Vector(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
                m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
                m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}


inline void Transform::operator()(const Vector &v, Vector *vt) const 
{
  float x = v.x, y = v.y, z = v.z;
  vt->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z;
  vt->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z;
  vt->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z;
}

// Transform Method Definitions
void Transform::Print(FILE *f) const 
{
    m.Print(f);
}



Transform Transform::Translate(const Vector &delta) {
    Matrix4x4 m(1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z,
                0, 0, 0,       1);
    Matrix4x4 minv(1, 0, 0, -delta.x,
                   0, 1, 0, -delta.y,
                   0, 0, 1, -delta.z,
                   0, 0, 0,        1);
    return Transform(m, minv);
}


Transform Transform::Scale(float x, float y, float z) {
    Matrix4x4 m(x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1);
    Matrix4x4 minv(1.f/x,     0,     0,     0,
                   0,     1.f/y,     0,     0,
                   0,         0,     1.f/z, 0,
                   0,         0,     0,     1);
    return Transform(m, minv);
}


Transform Transform::RotateX(float angle) {
    float sin_t = sinf(Radians(angle));
    float cos_t = cosf(Radians(angle));
    Matrix4x4 m(1,     0,      0, 0,
                0, cos_t, -sin_t, 0,
                0, sin_t,  cos_t, 0,
                0,     0,      0, 1);
    return Transform(m, Transpose(m));
}


Transform Transform::RotateY(float angle) {
    float sin_t = sinf(Radians(angle));
    float cos_t = cosf(Radians(angle));
    Matrix4x4 m( cos_t,   0,  sin_t, 0,
                 0,   1,      0, 0,
                -sin_t,   0,  cos_t, 0,
                 0,   0,   0,    1);
    return Transform(m, Transpose(m));
}



Transform Transform::RotateZ(float angle) {
    float sin_t = sinf(Radians(angle));
    float cos_t = cosf(Radians(angle));
    Matrix4x4 m(cos_t, -sin_t, 0, 0,
                sin_t,  cos_t, 0, 0,
                0,      0, 1, 0,
                0,      0, 0, 1);
    return Transform(m, Transpose(m));
}


Transform Transform::Rotate(float angle, const Vector &axis) {

    Vector a = Vector::Normalize(axis);
    float s = sinf(Radians(angle));
    float c = cosf(Radians(angle));
    float m[4][4];

    m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
    m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
    m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
    m[0][3] = 0;

    m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
    m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
    m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
    m[1][3] = 0;

    m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
    m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
    m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    Matrix4x4 mat(m);
    return Transform(mat, Transpose(mat));
}


Transform Transform::LookAt(const Point &pos, const Point &look, const Vector &up) {
    float m[4][4];
    // Initialize fourth column of viewing matrix
    m[0][3] = pos.x;
    m[1][3] = pos.y;
    m[2][3] = pos.z;
    m[3][3] = 1;

    // Initialize first three columns of viewing matrix
    Vector dir = Vector::Normalize(look - pos);
    Vector left = Vector::Normalize(Vector::Cross(Vector::Normalize(up), dir));
    Vector newUp = Vector::Cross(dir, left);
    m[0][0] = left.x;
    m[1][0] = left.y;
    m[2][0] = left.z;
    m[3][0] = 0.;
    m[0][1] = newUp.x;
    m[1][1] = newUp.y;
    m[2][1] = newUp.z;
    m[3][1] = 0.;
    m[0][2] = dir.x;
    m[1][2] = dir.y;
    m[2][2] = dir.z;
    m[3][2] = 0.;
    Matrix4x4 camToWorld(m);
    return Transform(Inverse(camToWorld), camToWorld);
}