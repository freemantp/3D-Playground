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

GLSLProgram::GLSLProgram() : programHandle(0), linked(false) { }

GLSLProgram::~GLSLProgram()
{
	if ( programHandle > 0 ) {
		glDeleteProgram(programHandle);
	}
}

bool GLSLProgram::compileShaderFromString( const string & source, GLSLShader::GLSLShaderType type )
{
    if( programHandle <= 0 ) 
	{
        programHandle = glCreateProgram();
        if( programHandle == 0) 
		{
            logString = "Unable to create shader program.";
            return false;
        }
    }

    GLuint shaderHandle = 0;

	switch( type ) 
	{
		case GLSLShader::VERTEX:
			shaderHandle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case GLSLShader::FRAGMENT:
			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case GLSLShader::GEOMETRY:
			shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case GLSLShader::TESS_CONTROL:
			shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case GLSLShader::TESS_EVALUATION:
			shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		default:
			return false;
	}

    const GLchar * c_code = source.c_str();
    glShaderSource( shaderHandle, 1, &c_code, NULL );

    // Compile the shader
    glCompileShader(shaderHandle );

    // Check for errors
    GLint result;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );

    if( GL_FALSE == result ) {
        // Compile failed, store log and return false
        int length = 0;
        logString = "";
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }

        return false;
    } else {
        // Compile succeeded, attach shader and return true
		glAttachShader(programHandle, shaderHandle);
        return true;
    }
}

bool GLSLProgram::link()
{
    if( linked ) return true;
	if( programHandle <= 0 ) return false;

    glLinkProgram(programHandle);

    int status = 0;
    glGetProgramiv( programHandle, GL_LINK_STATUS, &status);
    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        logString = "";

        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 ) {
            GLchar * c_log = new GLchar[length];
            GLsizei written = 0;
            glGetProgramInfoLog(programHandle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }

        return false;
    } else {
        linked = true;
        return linked;
    }
}

GLuint GLSLProgram::getProgramHandle() 
{ 
	return programHandle; 
};

string GLSLProgram::log()
{
    return logString;
}

void GLSLProgram::use()
{
	if( programHandle <= 0 || (! linked) ) 
		return;

	glUseProgram(programHandle);
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
}

void GLSLProgram::setUniform(const GLchar *name, float value)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniform1f(loc, value);
	} 
	else 
	{
#if _DEBUG
		fprintf(stderr, "Error setting parameter '%s'\n", name);
#endif
	}
}

void GLSLProgram::setUniform(const GLchar *name, int value)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniform1i(loc, value);
	} 
	else 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
#endif
	}
}

void GLSLProgram::setUniform(const GLchar *name, const vec2& v)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniform2f(loc, v.x, v.y);
	} 
	else 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
#endif
	}
}

void GLSLProgram::setUniform(const GLchar *name, const vec3& v)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniform3f(loc, v.x, v.y, v.z);
	} 
	else 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
#endif
	}
}

void GLSLProgram::setUniform(const char *name,  const vec4& v)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	} 
	else 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
#endif
	}
}

void GLSLProgram::setUniform(const GLchar *name, const mat3& m)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	} 
	else 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
#endif
	}
}

void GLSLProgram::setUniform(const GLchar *name, const mat4& m)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) 
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	} 
	else 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
#endif
	}
}

void GLSLProgram::setUniformArray(const GLchar *name, GLfloat *v, int elementSize, int count)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc == -1) 
	{
#if _DEBUG
		Error(string("Error setting parameter '") + name + "'");
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

void GLSLProgram::bindTexture(const GLchar *name, GLuint tex, GLenum target, GLint unit)
{
	GLint loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, tex);
		glUseProgram(programHandle);
		glUniform1i(loc, unit);
		glActiveTexture(GL_TEXTURE0);
	} else {
#if _DEBUG
		fprintf(stderr, "Error binding texture '%s'\n", name);
#endif
	}
}


std::vector<std::string> GLSLProgram::getVertexAttributes()
{
	GLint nAttribs, maxLength, charsWritten, size;
	GLenum type;

	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES,&nAttribs);
	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&maxLength);

	GLchar* currentAttrib = new GLchar[maxLength];

	std::vector<std::string> attribNames(nAttribs);

	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveAttrib(programHandle, i, maxLength, &charsWritten, &size, &type, currentAttrib);
		attribNames.push_back(std::string(currentAttrib));
	}

	delete[] currentAttrib;
	return attribNames;

}

std::vector<std::string> GLSLProgram::getUniformAttributes()
{
	GLint nAttribs, maxLength, charsWritten, size;
	GLenum type;

	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS,&nAttribs);
	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLength);

	GLchar* currentAttrib = new GLchar[maxLength];

	std::vector<std::string> attribNames(nAttribs);

	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveUniform(programHandle, i, maxLength, &charsWritten, &size, &type, currentAttrib);
		attribNames.push_back(std::string(currentAttrib));
	}

	delete[] currentAttrib;
	return attribNames;

}

///// FILE utils

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