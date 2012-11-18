#include "stdafx.h"
#include "PhongTextureShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"

#include  <IL/il.h>

PhongTextureShader::PhongTextureShader(const string& textureFile)
: PhongShader("phongTexShader")
{
	string texturePath = Config::TEXTURE_BASE_PATH + textureFile;

	if( ilLoadImage(texturePath.c_str()) == IL_TRUE )
	{

		if( ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE) == IL_TRUE )
		{
			ILubyte* mem = ilGetData();
			int width =  ilGetInteger(IL_IMAGE_WIDTH);
			int height =  ilGetInteger(IL_IMAGE_HEIGHT);

			glActiveTexture(GL_TEXTURE0);
		
			glGenTextures(1,&texObjHandle);

			glBindTexture(GL_TEXTURE_2D, texObjHandle);

			glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,width, height,0,GL_RGBA, GL_UNSIGNED_BYTE, mem);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			
			
			std::cout << "success" << std::endl;
		} 
		else 
			Error("Could not convert texture: " + texturePath);
	}
	else
		Error("Could not load texture: " + texturePath);
}

PhongTextureShader::~PhongTextureShader()
{
	glDeleteTextures(1,&texObjHandle);
}

void PhongTextureShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	setLightAndModel(scene);
	setUniform("Shininess", shininess);
	//setUniform("Tex1", texObjHandle);
}