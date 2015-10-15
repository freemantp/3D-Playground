#pragma once;

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <glm/gtc/constants.hpp>

/**
  See Sampling with Hammersley and Halton Points 
  http://www.cse.cuhk.edu.hk/~ttwong/papers/udpoint/udpoints.html

  \TODO: This might be obsolete because of glm::sphericalRand
*/
class HaltonSphere
{

public:

	/// Halton point set generation, two p-adic Van der put sequences, useful for incremental approach
	static void GenerateRays(std::vector<glm::vec3> &points, int p2=7);

	/// Rotates each vector with a random euler matrix
	static void ShuffleRays(std::vector<glm::vec3>& rays);

};