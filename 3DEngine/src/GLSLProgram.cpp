/*
* Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
*
* Please refer to the NVIDIA end user license agreement (EULA) associated
* with this source code for terms and conditions that govern your use of
* this software. Any use, reproduction, disclosure, or distribution of
* this software and related documentation outside the terms of the EULA
* is strictly prohibited.
*
*/

#include "stdafx.h"
#include "GLSLProgram.h"
#include <cstdlib>
#include <string>

GLSLProgram::GLSLProgram(const char *vsource, const char *fsource)
{
	program = compileProgram(vsource, 0, fsource);
}

GLSLProgram::GLSLProgram(const char *vsource, const char *gsource, const char *fsource,
						 GLenum gsInput, GLenum gsOutput)
{
	program = compileProgram(vsource, gsource, fsource, gsInput, gsOutput);
}

GLSLProgram::~GLSLProgram()
{
	if (program) {
		glDeleteProgram(program);
	}
}

void GLSLProgram::enable()
{
	glUseProgram(program);
}

void GLSLProgram::disable()
{
	glUseProgram(0);
}

void GLSLProgram::setUniform1f(const char *name, float value)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glUniform1f(loc, value);
	} else {
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void
	GLSLProgram::setUniform1i(const GLchar *name, GLint value)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glUniform1i(loc, value);
	} else {
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void GLSLProgram::setUniform2f(const char *name, float x, float y)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glUniform2f(loc, x, y);
	} else {
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void GLSLProgram::setUniform3f(const char *name, float x, float y, float z)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glUniform3f(loc, x, y, z);
	} else {
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void GLSLProgram::setUniform4f(const char *name, float x, float y, float z, float w)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glUniform4f(loc, x, y, z, w);
	} else {
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void GLSLProgram::setUniformMatrix4fv(const GLchar *name, GLfloat *m, bool transpose)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glUniformMatrix4fv(loc, 1, transpose, m);
	} else {
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void GLSLProgram::setUniformfv(const GLchar *name, GLfloat *v, int elementSize, int count)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc == -1) {
#ifdef _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
		return;
	}

	switch (elementSize) {
	case 1:
		glUniform1fv(loc, count, v);
		break;
	case 2:
		glUniform2fv(loc, count, v);
		break;
	case 3:
		glUniform3fv(loc, count, v);
		break;
	case 4:
		glUniform4fv(loc, count, v);
		break;
	}
}

void GLSLProgram::bindTexture(const char *name, GLuint tex, GLenum target, GLint unit)
{
	GLint loc = glGetUniformLocation(program, name);
	if (loc >= 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, tex);
		glUseProgram(program);
		glUniform1i(loc, unit);
		glActiveTexture(GL_TEXTURE0);
	} else {
#if _DEBUG
		fprintf(stderr, "Error binding texture '%s'\n", name);
#endif
	}
}


GLboolean GLSLProgram::checkCompileStatus(GLuint shader, GLint *status)
{
	glGetShaderiv(shader, GL_COMPILE_STATUS, status);
	if (!(*status)) {
		/*char log[2048];
		int len;
		glGetShaderInfoLog(shader, 2048, (GLsizei*)&len, log);
		printf("Error: shader(%04d), Info log: %s\n", (int)shader, log);
		glDeleteShader(shader);*/
		return GL_FALSE;
	}
	return GL_TRUE;
}

std::string GLSLProgram::getErrorLog(GLint id, bool isShader = true)
{
	GLsizei logLength;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

	if(logLength > 0)
	{
		
		GLchar* errorMsg = new GLchar[logLength];
		GLsizei writtenLength;
		if(isShader)
			glGetShaderInfoLog(id,logLength,&writtenLength,errorMsg);
		else
			glGetProgramInfoLog(id,logLength,&writtenLength,errorMsg);

		std::string s(errorMsg);
		delete[] errorMsg;

		return s;
	}

	return std::string();
}


GLuint GLSLProgram::compileProgram(const GLchar *vsource, const GLchar *gsource, const GLchar *fsource,
								   GLenum gsInput, GLenum gsOutput)
{
	GLuint vertexShader   = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLint compiled = 0;

	glShaderSource(vertexShader, 1, &vsource, NULL);
	glShaderSource(fragmentShader, 1, &fsource, NULL);

	glCompileShader(vertexShader);
	if (checkCompileStatus(vertexShader, &compiled) == GL_FALSE) {
		
		std::string errorMsg = getErrorLog(vertexShader);
		std::cerr << "<compileProgram compilation error with vertexShader>:\n"  << errorMsg << std::endl; 
		return 0;
	}

	glCompileShader(fragmentShader);
	if (checkCompileStatus(fragmentShader, &compiled) == GL_FALSE) {

		std::string errorMsg = getErrorLog(fragmentShader);
		std::cerr << "<compileProgram compilation error with fragmentShader>:\n" << errorMsg << std::endl; 

		return 0;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	GLuint geomShader;
	bool useGeomShader = (gsource != NULL);

	if (useGeomShader) {
		geomShader = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		glShaderSource(geomShader, 1, &gsource, 0);
		glCompileShader(geomShader);
		glGetShaderiv(geomShader, GL_COMPILE_STATUS, (GLint*)&compiled);

		if (checkCompileStatus(geomShader, &compiled) == GL_FALSE) {
			
			std::string errorMsg = getErrorLog(geomShader);			
			std::cerr << "<compileProgram compilation error with geomShader>:\n" << errorMsg << std::endl; 

			return 0;
		}

		glAttachShader(program, geomShader);

		glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, gsInput);
		glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, gsOutput); 
		glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, 4); 
	}

	glLinkProgram(program);

	// check if program linked
	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
			
		std::string errorMsg = getErrorLog(program,false);			
		std::cerr << "<Failed to link program>:\n" << errorMsg << std::endl; 

		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if(useGeomShader)
			glDeleteShader(geomShader);

		program = 0;
	}

	return program;
}

unsigned long GLSLProgram::getFileLength(std::ifstream& file)
{
	if(!file.good()) return 0;

	unsigned long pos=(unsigned long)file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = (unsigned long)file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

const char* GLSLProgram::loadSource(char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in);
	/*if(!file) 
	return -1;*/

	unsigned long len = getFileLength(file);

	/*if (len==0) 
	return -2;   // "Empty File" */

	GLubyte* ShaderSource = (GLubyte*) new char[len+1];
	/*if (ShaderSource == 0) 
	return -3;   // can't reserve memory*/

	ShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	unsigned int i=0;
	while (file.good())
	{
		ShaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}

	ShaderSource[i] = 0;  // 0 terminate it.

	file.close();

	return (const char*)ShaderSource;
}


