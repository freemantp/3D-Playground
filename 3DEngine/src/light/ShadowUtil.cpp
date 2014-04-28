#include "stdafx.h"
#include "ShadowUtil.h"


#include <random>
#include <chrono>

#include "../texture/Texture3D.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Texture3D_ptr ShadowUtil::BuildRandShadowOffset(unsigned int size, unsigned  int samplesU, unsigned  int samplesV)
{
	unsigned int seed = static_cast<unsigned int >(std::chrono::system_clock::now().time_since_epoch().count());

	std::default_random_engine generator(seed);
	std::uniform_real_distribution<float> jitter(-0.5f, 0.5f);

	double randNumber = jitter(generator);
	unsigned int numSamples = samplesU * samplesV;

	float* dataBuffer = new float[size * size * numSamples * 2];

	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
		{
			for (unsigned int k = 0; k < numSamples; k += 2)
			{
				int x1 = k % samplesU;
				int x2 = (k+1) % samplesU;
				int y1 = (numSamples - 1 - k) / samplesU;
				int y2 = (numSamples - 2 - k) / samplesU;

				glm::vec4 v;
				v.x = ( (x1 + 0.5f) + jitter(generator) ) / samplesU;
				v.y = ( (y1 + 0.5f) + jitter(generator) ) / samplesV;
				v.z = ( (x2 + 0.5f) + jitter(generator) ) / samplesU;
				v.w = ( (x2 + 0.5f) + jitter(generator) ) / samplesV;

				float twoPi = glm::pi<float>() * 2;

				int cell = ((k / 2) * size * size + j * size + i) * 4;

				dataBuffer[cell + 0] = sqrtf(v.y) * cosf(twoPi*v.x);
				dataBuffer[cell + 1] = sqrtf(v.y) * sinf(twoPi*v.x);
				dataBuffer[cell + 2] = sqrtf(v.w) * cosf(twoPi*v.z);
				dataBuffer[cell + 3] = sqrtf(v.w) * sinf(twoPi*v.z);
			}
		}

	}

	Texture3D_ptr tex = Texture3D::Create(size, size, 2 * numSamples, Texture::Format::RGBA32F);
	tex->SetData(dataBuffer);

	delete[] dataBuffer;

	return tex;
}