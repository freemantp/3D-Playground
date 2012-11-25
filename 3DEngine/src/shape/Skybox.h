#pragma once
#include "Box.h"

#include <string>

class CubeMapTexture;

class Skybox :
	public Box
{
public:
	Skybox(const std::string& cubeMapBase,const std::string& type);
	~Skybox();

protected:
	virtual void setShader(ShaderBase* shader);

	CubeMapTexture* texture;

};

