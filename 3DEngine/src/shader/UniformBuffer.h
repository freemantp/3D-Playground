#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

class GLSLProgram;

using glm::vec3;
using glm::vec4;

class UniformBuffer
{
public:
	///The glsl program and bufferName is needed to allocate the right amount of mem (queried)
	UniformBuffer(GLSLProgram* program, std::string bufferName, const GLchar* elemNames[], int numElems);
	~UniformBuffer(void);

	/// Updates the buffer object with the content set trough the members of this class
	void updateBufferObject();
	void bindToShader(GLSLProgram* program, std::string bufferName);

	void setElement(const std::string& name, const vec3& v);
	void setElement(const std::string& name, const vec4& v);

protected:

	//virtual void 
	std::map<std::string,GLint> offsets;
	GLint blockSize;
	GLuint uboHandle;
	GLubyte* blockBuffer;
};

