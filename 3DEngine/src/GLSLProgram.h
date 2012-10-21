 // Simple class to contain GLSL shaders/programs
#pragma once

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::string;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

namespace GLSLShader 
{
    enum GLSLShaderType 
	{
        VERTEX, FRAGMENT, GEOMETRY,
        TESS_CONTROL, TESS_EVALUATION
    };
};

class GLSLProgram
{
public:

	GLSLProgram();
	~GLSLProgram();

	bool compileShaderFromString( const string & source, GLSLShader::GLSLShaderType type );
	bool link();
	bool isLinked() const;
	string log() const;

	void use();
	void unuse();

	void setUniform(const GLchar *name, int value);
	void setUniform(const GLchar *name, float value);
	void setUniform(const GLchar *name, bool value);
	void setUniform(const GLchar *name, const vec2& v);
    void setUniform(const GLchar *name, const vec3& v);
	void setUniform(const GLchar *name, const vec4& v);
	void setUniform(const GLchar *name, const mat4& m);
    void setUniform(const GLchar *name, const mat3& m);
	void setUniformArray(const GLchar *name, float *v, int elementSize, int count=1);
	void bindTexture(const char *name, GLuint tex, GLenum target, GLint unit);

	static const char* loadSource(char* filename);

	GLuint getProgramHandle() const;

	std::vector<std::string>  getVertexAttributes();
	std::vector<std::string>  getUniformAttributes();

	void bindAttribLocation( GLuint location, const char * name);
    void bindFragDataLocation( GLuint location, const char * name );
	
protected:

	GLuint programHandle;
	string logString;
	bool linked;

	static unsigned long getFileLength(std::ifstream& file);
};


