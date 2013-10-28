#pragma once
#include "Box.h"
#include <string>

SHARED_PTR_CLASS_DECL(Skybox);
SHARED_PTR_CLASS_DECL(CubeMapTexture);


class Skybox : public Box
{
public:

	Skybox(CubeMapTexture_ptr texture);
	~Skybox();

	CubeMapTexture_ptr texture;
protected:		
	
	virtual void setShader(ShaderBase_ptr shader);

	

};

