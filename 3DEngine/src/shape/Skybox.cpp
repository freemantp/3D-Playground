#include "stdafx.h"
#include "Skybox.h"
#include "../shader/SkyboxShader.h"
#include "../texture/CubeMapTexture.h"

#include <glm/gtc/matrix_transform.hpp>

Skybox::Skybox(const std::string& cubeMapBase,const std::string& type)
	: Box()
{
	worldTransform = glm::scale(glm::mat4(1.0),glm::vec3(50,50,50));	
	texture = new CubeMapTexture(Config::TEXTURE_BASE_PATH + cubeMapBase,type);

	setShader(new SkyboxShader(texture));

}


Skybox::~Skybox()
{
	delete shaderProgram;
	delete texture;
}

void Skybox::setShader(ShaderBase* shader)
{
	Box::setShader(shader);
}