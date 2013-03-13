#pragma once
#include "Box.h"
#include <string>

class CubeMapTexture;

SHARED_PTR_CLASS_DECL(Skybox);

class Skybox : public Box
{
public:

	
	Skybox(CubeMapTexture* texture);
	~Skybox();

	CubeMapTexture* texture;
protected:		
	
	virtual void setShader(ShaderBase* shader);

	

};

