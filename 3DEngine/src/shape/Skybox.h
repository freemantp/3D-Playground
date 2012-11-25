#pragma once
#include "Box.h"

#include <string>

class CubeMapTexture;

class Skybox :
	public Box
{
public:
	Skybox(CubeMapTexture* texture);
	~Skybox();

protected:
	virtual void setShader(ShaderBase* shader);

	CubeMapTexture* texture;

};

