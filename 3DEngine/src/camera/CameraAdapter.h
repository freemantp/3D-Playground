#pragma once

#include "../camera/Camera.h"

using glm::vec2;

class CameraAdapter
{

protected:
	CameraAdapter(Camera& cam) :cam(cam) {  };

	Camera& cam;

};

