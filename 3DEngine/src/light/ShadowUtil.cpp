#include "stdafx.h"
#include "ShadowUtil.h"


#include <random>
#include <chrono>

#include "../texture/Texture3D.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Texture3D_ptr ShadowUtil::GenerateCircularOffsets(unsigned int size, unsigned  int samplesPhi, unsigned  int samplesR)
{
	unsigned int seed = static_cast<unsigned int >(std::chrono::system_clock::now().time_since_epoch().count());

	typedef float value_t;

	std::default_random_engine generator(seed);
	std::uniform_real_distribution<value_t> jitter(-0.5f, 0.5f);

	unsigned int numSamples = samplesPhi * samplesR;

	auto dataBuffer = std::unique_ptr<value_t[]>(new value_t[size * size * numSamples * 2]);

	const value_t twoPi = glm::pi<value_t>() * 2;

	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
		{
			for (unsigned int k = 0; k < numSamples; k += 2)
			{
				unsigned int x1 = k % samplesPhi;
				unsigned int x2 = (k + 1) % samplesPhi;
				unsigned int y1 = (numSamples - 1 - k) / samplesPhi;
				unsigned int y2 = (numSamples - 1 - k - 1) / samplesPhi;

				glm::vec4 v;
				v.x = ((x1 + 0.5f) + jitter(generator)) / samplesPhi;
				v.y = ((y1 + 0.5f) + jitter(generator)) / samplesR;
				v.z = ((x2 + 0.5f) + jitter(generator)) / samplesPhi;
				v.w = ((y2 + 0.5f) + jitter(generator)) / samplesR;

				assert(v.x >= 0.f && v.x <= 1.f);
				assert(v.y >= 0.f && v.y <= 1.f);
				assert(v.z >= 0.f && v.z <= 1.f);
				assert(v.w >= 0.f && v.w <= 1.f);

				unsigned int cell = ((k / 2) * size * size + j * size + i) * 4;

				dataBuffer[cell + 0] = sqrtf(v.y) * cosf(twoPi*v.x);
				dataBuffer[cell + 1] = sqrtf(v.y) * sinf(twoPi*v.x);
				dataBuffer[cell + 2] = sqrtf(v.w) * cosf(twoPi*v.z);
				dataBuffer[cell + 3] = sqrtf(v.w) * sinf(twoPi*v.z);

										   
				assert(dataBuffer[cell + 0] >= -1.f && dataBuffer[cell + 0] <= 1.f);				
				assert(dataBuffer[cell + 1] >= -1.f && dataBuffer[cell + 1] <= 1.f);				
				assert(dataBuffer[cell + 2] >= -1.f && dataBuffer[cell + 2] <= 1.f);				
				assert(dataBuffer[cell + 3] >= -1.f && dataBuffer[cell + 3] <= 1.f);

			}

		}
	}	

	Texture3D_ptr tex = Texture3D::Create(size, size, numSamples / 2, dataBuffer.get(), Texture::Format::RGBA32F);;

	return tex;
}