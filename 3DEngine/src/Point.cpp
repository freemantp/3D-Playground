#include "stdafx.h"
#include "Point.h"
#include <cassert>


Point::Point(void)  : x(0.0f), y(0.0f), z(0.0f)
{
}


Point::~Point(void)
{
}

Point::Point(float xx, float yy, float zz) : x(xx), y(yy), z(zz) 
{
    assert(!HasNaNs());
}

#ifndef NDEBUG
Point::Point(const Point &p) {
    assert(!p.HasNaNs());
    x = p.x; y = p.y; z = p.z;
}
    
Point& Point::operator=(const Point &p) {
    assert(!p.HasNaNs());
    x = p.x; y = p.y; z = p.z;
    return *this;
}
#endif // !NDEBUG

Point Point::operator+(const Vector &v) const {
    assert(!v.HasNaNs());
    return Point(x + v.x, y + v.y, z + v.z);
}

Point& Point::operator+=(const Vector &v) {
    assert(!v.HasNaNs());
    x += v.x; y += v.y; z += v.z;
    return *this;
}
Vector Point::operator-(const Point &p) const {
    assert(!p.HasNaNs());
    return Vector(x - p.x, y - p.y, z - p.z);
}
    
Point Point::operator-(const Vector &v) const {
    assert(!v.HasNaNs());
    return Point(x - v.x, y - v.y, z - v.z);
}
    
Point& Point::operator-=(const Vector &v) {
    assert(!v.HasNaNs());
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}
Point& Point::operator+=(const Point &p) {
    assert(!p.HasNaNs());
    x += p.x; y += p.y; z += p.z;
    return *this;
}
Point Point::operator+(const Point &p) const {
    assert(!p.HasNaNs());
    return Point(x + p.x, y + p.y, z + p.z);
}

Point Point::operator* (float f) const {
    return Point(f*x, f*y, f*z);
}
Point& Point::operator*=(float f) {
    x *= f; y *= f; z *= f;
    return *this;
}
Point Point::operator/ (float f) const {
    float inv = 1.f/f;
    return Point(inv*x, inv*y, inv*z);
}
Point& Point::operator/=(float f) {
    float inv = 1.f/f;
    x *= inv; y *= inv; z *= inv;
    return *this;
}
float Point::operator[](int i) const {
    assert(i >= 0 && i <= 2);
    return (&x)[i];
}
    
float& Point::operator[](int i) {
    assert(i >= 0 && i <= 2);
    return (&x)[i];
}
bool Point::HasNaNs() const {
    return isnan(x) || isnan(y) || isnan(z);
}

bool Point::operator==(const Point &p) const {
    return x == p.x && y == p.y && z == p.z;
}
bool Point::operator!=(const Point &p) const {
    return x != p.x || y != p.y || z != p.z;
}