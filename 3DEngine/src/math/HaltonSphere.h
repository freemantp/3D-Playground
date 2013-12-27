#pragma once;

#include <glm/core/type.hpp>
#include <vector>
#include <random>

/**
  See Sampling with Hammersley and Halton Points 
  http://www.cse.cuhk.edu.hk/~ttwong/papers/udpoint/udpoints.html
*/
class HaltonSphere
{

public:

	/// Halton point set generation, two p-adic Van der Corport sequences,Useful for incremental approach
	static void GenerateRays(std::vector<glm::vec3> &points, int p2=7);

	/// Rotates each vector with a random euler matrix
	static void ShuffleRays(std::vector<glm::vec3>& rays);

};