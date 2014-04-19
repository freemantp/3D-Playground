#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Texture3D);

class ShadowUtil
{
public:

	static Texture3D_ptr BuildRandShadowOffset(unsigned int size, unsigned  int samplesU, unsigned  int samplesV);


};

