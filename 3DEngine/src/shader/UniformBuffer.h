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
	UniformBuffer(const GLSLProgram* program, std::string bufferName, const GLchar* elemNames[], int numElems);
	~UniformBuffer(void);

	void bindToShader(GLSLProgram* program, std::string bufferName);

	void setElement(const std::string& name, const vec3& v);
	void setElement(const std::string& name, const vec4& v);

protected:

	void printUniforms(const GLchar* elemNames[],const GLuint* indices,const GLint* eOffsets);
	inline void setElement(const std::string& name, const void* ptr, const GLsizei numBytes);

	std::map<std::string,GLint> offsets;
	GLuint uboHandle;
};

