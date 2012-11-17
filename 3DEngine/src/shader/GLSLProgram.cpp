#include "stdafx.h"
#include "GLSLProgram.h"
#include <string>

GLSLProgram::GLSLProgram() 
	: programHandle(0)
	, linked(false) 
{
	programHandle = glCreateProgram();

	if( programHandle == 0) 
	{
		logString = "Unable to create shader program";
	}   
}

GLSLProgram::~GLSLProgram()
{
	if ( programHandle > 0 ) 
	{
		glDeleteProgram(programHandle);
	}
}

bool GLSLProgram::compileShaderFromString( const string& source,  
										  GLSLShader::GLSLShaderType type)
{
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
	case GLSLShader::TESSELATION_CONTROL:
		shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case GLSLShader::TESSELATION_EVALUATION:
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

	if( GL_FALSE == result ) 
	{       
		logString = getInfoLog(shaderHandle);
		return false;
	} 
	else 
	{
		// Compile succeeded, attach shader and return true
		glAttachShader(programHandle, shaderHandle);
		return true;
	}
}

bool GLSLProgram::link()
{
	if( linked ) 
		return true;

	if( programHandle <= 0 ) 
	{
		logString = "Invalid program handle";
		return false;
	}

	glLinkProgram(programHandle);

	int status = 0;
	glGetProgramiv( programHandle, GL_LINK_STATUS, &status);
	if( GL_TRUE != status ) 
	{
		logString = getInfoLog(programHandle);
		return false;
	} else {
		linked = true;
		return linked;
	}
}

bool GLSLProgram::isLinked() const
{
	return linked;
}

bool GLSLProgram::validate()
{
	glValidateProgram(programHandle);
	GLint status;
	glGetProgramiv(programHandle, GL_VALIDATE_STATUS, &status);

	logString = getInfoLog(programHandle);

	return GL_TRUE == status;
}

GLuint GLSLProgram::getProgramHandle() const
{ 
	return programHandle; 
};

string GLSLProgram::log() const
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
		Error(string("Error setting parameter '") + name + "'");
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

void GLSLProgram::setUniform(const GLchar *name, unsigned int value)
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

void GLSLProgram::setUniformArray(const GLchar *name, 
								  GLfloat *v, 
								  int elementSize, 
								  int count)
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

void GLSLProgram::bindTexture(const GLchar *name, 
							  GLuint tex, 
							  GLenum target,
							  GLint unit)
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
		Error(string("Error binding texture  '") + name + "'");
#endif
	}
}

std::vector<std::string> GLSLProgram::getVertexAttributes()
{
	GLint nAttribs, maxLength, charsWritten, size;
	GLenum type;

	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &nAttribs);
	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	GLchar* currentAttrib = new GLchar[maxLength];

	std::vector<std::string> attribNames(nAttribs);

	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveAttrib(programHandle, i, maxLength, &charsWritten, 
			&size, &type, currentAttrib);

		attribNames.push_back( std::string(currentAttrib) );
	}

	delete[] currentAttrib;
	return attribNames;

}

std::vector<std::string> GLSLProgram::getUniformAttributes() const
{
	GLint nAttribs, maxLength, charsWritten, size;
	GLenum type;

	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS,&nAttribs);
	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLength);

	GLchar* currentAttrib = new GLchar[maxLength];

	std::vector<std::string> attribNames(nAttribs);

	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveUniform( programHandle, i, maxLength, &charsWritten, 
			&size, &type, currentAttrib );

		attribNames.push_back( std::string(currentAttrib) );
	}

	delete[] currentAttrib;
	return attribNames;

}

bool GLSLProgram::bindAttribLocation( GLuint index,  std::string& name)
{
	if(linked) 
		return false;

	glBindAttribLocation( programHandle, index, name.c_str() );
	return true;

}

bool GLSLProgram::bindFragDataLocation( GLuint colorNumber,  std::string& name)
{
	if(linked) 
		return false;

	glBindFragDataLocation( programHandle, colorNumber, name.c_str() );
	return true;

}

string GLSLProgram::getInfoLog(GLuint handle) 
{

	GLboolean isShader = glIsShader(handle) ;

	// Compile failed, store log and return false
	int length = 0;
	string  logString = "";

	if(GL_TRUE == isShader)
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length );
	else
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

	if(length > 0 ) {
		GLchar * c_log = new GLchar[length];
		int written = 0;

		if(isShader)
			glGetShaderInfoLog(handle, length, &written, c_log);
		else
			glGetProgramInfoLog(handle, length, &written, c_log);


		logString = c_log;
		delete [] c_log;
	}

	return logString;
}