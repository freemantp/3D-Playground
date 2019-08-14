#include "stdafx.h"

#include "EmergencyShape.h"
#include "../materials/Material.h"
#include "../shader/MaterialShader.h"
#include "../shader/ShaderLibrary.h"

#include <glm/glm.hpp>

void EmergencyShape::Render(const Scene_ptr& scene) const
{
	ShaderLibrary_ptr sl = ShaderLibrary::Instance();
	if (MaterialShader_ptr current_shader = sl->ShaderLookup(material))
	{
		current_shader->SetMaterial(material);
		current_shader->Use(scene, glm::mat4(1.0f));

		/* tell GL to only draw onto a pixel if the shape is closer to the viewer
		than anything already drawn at that pixel */
		glEnable(GL_DEPTH_TEST); /* enable depth-testing */
								 /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
		glDepthFunc(GL_LESS);

		glCullFace(GL_FRONT);
		glBindVertexArray(vao);
		/* draw points 0-3 from the currently bound VAO with current in-use shader */
		glDrawArrays(GL_TRIANGLES, 0, 3);

		current_shader->UnUse();
	}
}

void EmergencyShape::RenderShadowMap(const ShadowMapShader_ptr&) const
{

}

void EmergencyShape::Init()
{
	GLuint vbo;

	/* geometry to use. these are 3 xyz points (9 floats total) to make a triangle */
	std::vector<glm::vec3> vertexData = {
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
	};

	/* a vertex buffer object (VBO) is created here. this stores an array of
	data on the graphics adapter's memory. in our case - the vertex points */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3* vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

	/* the vertex array object (VAO) is a little descriptor that defines which
	data from vertex buffer objects should be used as input variables to vertex
	shaders. in our case - use our only VBO, and say 'every three floats is a
	variable' */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/* "attribute #0 should be enabled when this vao is bound" */
	glEnableVertexAttribArray(0);
	/* this VBO is already bound, but it's a good habit to explicitly specify which
	VBO's data the following vertex attribute pointer refers to */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	/* "attribute #0 is created from every 3 variables in the above buffer, of type
	float (i.e. make me vec3s)" */
	glVertexAttribPointer(0, 3, static_cast<gl::GLenum>(GL_FLOAT), GL_FALSE, 0, NULL);
}
