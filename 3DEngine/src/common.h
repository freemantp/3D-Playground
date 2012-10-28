#pragma once

#include <cmath>
#include <cfloat>
#include <string>

#define isnan _isnan

#define uint32_t unsigned __int32

#define M_PI       3.14159265358979323846f

inline float Radians(float deg) {
    return ((float)M_PI/180.f) * deg;
}


inline float Degrees(float rad) {
    return (180.f/(float)M_PI) * rad;
}

class Config {

public:

	static const std::string SHADER_BASE_PATH;
};