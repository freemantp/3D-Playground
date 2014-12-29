#pragma once

#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(UniformBuffer);
SHARED_PTR_CLASS_DECL(GLSLProgram)

class UniformBuffer
{
public:
	///The glsl program and bufferName is needed to allocate the right amount of mem (queried)
	
	UniformBuffer(const GLSLProgram_ptr program, std::string bufferName, const std::vector<std::string>& names);

	~UniformBuffer(void);

	void BindToShader(GLSLProgram_ptr program, std::string bufferName);
	
	void SetElement(const std::string& name, float v);
	void SetElement(const std::string& name, const glm::vec3& v);
	void SetElement(const std::string& name, const glm::vec4& v);
	void SetElement(const std::string& name, const glm::mat4& v);

protected:
	

	void PrintUniforms(const GLSLProgram_ptr program, const std::vector<std::string>& elemNames, GLuint* indices, GLint* eOffsets);
	inline void SetElement(const std::string& name, const void* ptr, const GLsizei numBytes);

	std::map<std::string,GLint> offsets;
	GLuint uboHandle;
};

