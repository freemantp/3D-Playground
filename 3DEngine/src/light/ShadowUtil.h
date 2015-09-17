#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Texture3D);
SHARED_PTR_CLASS_DECL(Texture2D);

class ShadowUtil
{
public:

	/**
	* \brief Builds a 3D noise texture containing circular offsets with distance [-1,1] from the origin.
	* The resulting texture will be of dimension size,size,samplesU*samplesV/2, two samples are packed into one vec4
	*
	* See OpenGL 4.0 shading language Cookbook, Chapte 7 for details
	*
	* \param size The dimension of x and y of the resulting texture
	* \param samplesPhi The number of samples in angular direction
	* \param samplesR The number of samples in radial direction
	*/
	static Texture3D_ptr GenerateCircularOffsets(unsigned int size, unsigned  int samplesPhi, unsigned  int samplesR);

	static Texture2D_ptr GenerateBayerDitherData();
};

