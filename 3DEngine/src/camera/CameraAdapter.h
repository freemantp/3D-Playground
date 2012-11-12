#pragma once

class Camera;

using glm::vec2;

class CameraAdapter
{

protected:
	CameraAdapter(Camera& cam) :cam(cam) {  };

	Camera& cam;

};

