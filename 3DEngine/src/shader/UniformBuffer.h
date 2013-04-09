#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(UniformBuffer);
SHARED_PTR_CLASS_DECL(GLSLProgram)

using glm::vec3;
using glm::vec4;

class UniformBuffer
{
public:
	///The glsl program and bufferName is needed to allocate the right amount of mem (queried)
	
	UniformBuffer(const GLSLProgram* program, std::string bufferName, const GLchar* elemNames[], int numElems);

	~UniformBuffer(void);

	void bindToShader(GLSLProgram* program, std::string bufferName);
	
	void setElement(const std::string& name, float v);
	void setElement(const std::string& name, const vec3& v);
	void setElement(const std::string& name, const vec4& v);

protected:
	

	void printUniforms(const GLSLProgram* program, const GLchar* elemNames[],GLuint* indices, GLint* eOffsets, int numElems);
	inline void setElement(const std::string& name, const void* ptr, const GLsizei numBytes);

	std::map<std::string,GLint> offsets;
	GLuint uboHandle;
};

