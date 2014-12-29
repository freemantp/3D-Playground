#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(CameraAdapter);
SHARED_PTR_CLASS_DECL(Camera);

class CameraAdapter
{

protected:
	CameraAdapter(Camera_ptr cam) : cam(cam) {  };

	Camera_ptr cam;

};

