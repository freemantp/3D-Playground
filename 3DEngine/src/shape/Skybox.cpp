#include "stdafx.h"
#include "Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "../materials/Material.h"

#include <glm/gtc/matrix_transform.hpp>

Skybox::Skybox(SkyboxMaterial_ptr material) : Box()
{
	worldTransform = glm::scale(glm::mat4(1.0),glm::vec3(50,50,50));
	this->material = material;
}

Skybox::~Skybox()
{
	
}
