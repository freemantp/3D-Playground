#include "stdafx.h"

#include "Vector.h"
#include <cassert>

Vector::Vector() : x(0.0f), y(0.0f), z(0.0f)
{

}

Vector::Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

Vector::~Vector()
{
}

#ifndef NDEBUG
// The default versions of these are fine for release builds; for debug
// we define them so that we can add the assert checks.
Vector::Vector(const Vector &v) {
    assert(!v.HasNaNs());
    x = v.x; y = v.y; z = v.z;
}
    
Vector& Vector::operator=(const Vector &v) {
    assert(!v.HasNaNs());
    x = v.x; y = v.y; z = v.z;
    return *this;
}
#endif // !NDEBUG

Vector Vector::operator+(const Vector &v) const 
{
    assert(!v.HasNaNs());
    return Vector(x + v.x, y + v.y, z + v.z);
}
    
Vector& Vector::operator+=(const Vector &v) 
{
    assert(!v.HasNaNs());
    x += v.x; y += v.y; z += v.z;
    return *this;
}

Vector Vector::operator-(const Vector &v) const 
{
    assert(!v.HasNaNs());
    return Vector(x - v.x, y - v.y, z - v.z);
}
    
Vector& Vector::operator-=(const Vector &v) {
    assert(!v.HasNaNs());
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

Vector Vector::operator*(float f) const 
{ 
	return Vector(f*x, f*y, f*z); 
}
    
Vector& Vector::operator*=(float f) 
{
    assert(!isnan(f));
    x *= f; y *= f; z *= f;
    return *this;
}

Vector  Vector::operator/(float f) const 
{
    assert(f != 0);
    float inv = 1.f / f;
    return Vector(x * inv, y * inv, z * inv);
}
    
Vector&  Vector::operator/=(float f) 
{
    assert(f != 0);
    float inv = 1.f / f;
    x *= inv; y *= inv; z *= inv;
    return *this;
}

Vector  Vector::operator-() const 
{ 
	return Vector(-x, -y, -z); 
}

float Vector::operator[](int i) const 
{
    assert(i >= 0 && i <= 2);
    return (&x)[i];
}
    
float&  Vector::operator[](int i) 
{
    assert(i >= 0 && i <= 2);
    return (&x)[i];
}

float Vector::LengthSquared() const 
{ 
	return x*x + y*y + z*z; 
}

float Vector::Length() const 
{ 
	return sqrtf(LengthSquared()); 
}

bool Vector::HasNaNs() const 
{ 
	return isnan(x) || isnan(y) || isnan(z); 
}

Vector Vector::Cross(const Vector &v1, const Vector &v2) {
    assert(!v1.HasNaNs() && !v2.HasNaNs());
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector((float)((v1y * v2z) - (v1z * v2y)),
                  (float)((v1z * v2x) - (v1x * v2z)),
                  (float)((v1x * v2y) - (v1y * v2x)));
}

Vector Vector::Normalize(const Vector &v) {
	return v / v.Length(); 
}