#pragma once


using namespace std;

class Vector
{
public:
	Vector(void);
	Vector(float x, float y, float z);

#ifndef NDEBUG
    Vector(const Vector &p);
    Vector &operator=(const Vector &p);
#endif // !NDEBUG

	~Vector(void);

	Vector operator+(const Vector &v) const ;
    Vector& operator+=(const Vector &v);
    Vector operator-(const Vector &v) const ;
    
    Vector& operator-=(const Vector &v);
    Vector operator*(float f) const;
    
    Vector &operator*=(float f);
    Vector operator/(float f) const;    
    Vector &operator/=(float f); 
    Vector operator-() const;

    float operator[](int i) const;    
	float& operator[](int i) ;  
    
    float LengthSquared() const; 
    float Length() const;
	bool HasNaNs() const; 

	static Vector Normalize(const Vector &v);
	static Vector Cross(const Vector &v1, const Vector &v2);

public:
	float x, y, z;
};

