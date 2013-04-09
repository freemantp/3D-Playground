#include "stdafx.h"
#include "Skybox.h"
#include "../shader/SkyboxShader.h"
#include "../texture/CubeMapTexture.h"

#include <glm/gtc/matrix_transform.hpp>

Skybox::Skybox(CubeMapTexture_ptr texture)
	: texture(texture)
	, Box()
{
	worldTransform = glm::scale(glm::mat4(1.0),glm::vec3(50,50,50));
	SkyboxShader_ptr shader(new SkyboxShader(texture));
	setShader(shader);
}

Skybox::~Skybox()
{
	
}

void Skybox::setShader(ShaderBase_ptr shader)
{
	Box::setShader(shader);
}