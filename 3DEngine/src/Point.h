#pragma once

#include "Vector.h"

class Point {
public:
    // Point Public Methods
	Point();
	~Point();

    Point(float xx, float yy, float zz);
#ifndef NDEBUG
    Point(const Point &p);
    Point &operator=(const Point &p);
#endif // !NDEBUG

    Point operator+(const Vector &v) const;
   
    Point &operator+=(const Vector &v);
    Vector operator-(const Point &p) const;
  
    Point operator-(const Vector &v) const;
    
    Point &operator-=(const Vector &v);
    Point &operator+=(const Point &p);
    Point operator+(const Point &p) const;
	Point operator* (float f) const;
    Point &operator*=(float f);
    Point operator/ (float f) const;
    Point &operator/=(float f) ;
    float operator[](int i) const;
    float &operator[](int i) ;
    bool HasNaNs() const;

    bool operator==(const Point &p) const ;
    bool operator!=(const Point &p) const ;

    // Point Public Data
    float x, y, z;
};