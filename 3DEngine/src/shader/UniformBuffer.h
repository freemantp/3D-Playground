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
	
	UniformBuffer(const GLSLProgram_ptr program, std::string bufferName, const GLchar* elemNames[], int numElems);

	~UniformBuffer(void);

	void BindToShader(GLSLProgram_ptr program, std::string bufferName);
	
	void SetElement(const std::string& name, float v);
	void SetElement(const std::string& name, const vec3& v);
	void SetElement(const std::string& name, const vec4& v);

protected:
	

	void PrintUniforms(const GLSLProgram_ptr program, const GLchar* elemNames[],GLuint* indices, GLint* eOffsets, int numElems);
	inline void SetElement(const std::string& name, const void* ptr, const GLsizei numBytes);

	std::map<std::string,GLint> offsets;
	GLuint uboHandle;
};

