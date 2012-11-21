#pragma once

#include <vector>
#include <string>

#include "../stdafx.h"
#include "../enums.h"

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
		TESSELATION_CONTROL, TESSELATION_EVALUATION
	};
};

/**
* \class GLSLProgram
* \brief GLSL shader program class
* \author Michael Morandi
*
* A class that encapsulates a OpenGL shader program
* and provides means to compile , link  and bind GLSL
* shader programs.
*/
class GLSLProgram
{
public:

	///	Create a new GLSL program
	GLSLProgram();

	virtual ~GLSLProgram();

	/** \brief Compiles shader code and attaches it to the program 
	* \param source glsl source code
	* \param type the shader type of the code to be compiled
	* \return true iff the shader source has successfully been compiled
	* 
	* This method compiles the passed shader code and attaches
	* it to the current program. If compilation fails, the compiler log
	* is available through log()
	*/
	bool compileShaderFromString( const string& source, 
		GLSLShader::GLSLShaderType type );

	/**
	* \brief Links the attached shader objects
	* \return true iff the shader objects have successfully been linked
	* 
	* This method links all attached shader objects, if linking fails,
	* the linker log can be obtained trough log()
	*/
	bool link();

	/// Returns true if the 
	bool isLinked() const;

	/* \brief Validates the shader program
	* \return true if the program is vaild
	* 
	* The validation is up to the OpenGL vendor implememtation
	* and hints if the program is inefficient, suboptimal or 
	* failing to execute. The details can be obtained through log()
	*/
	bool validate();

	/// Returns the log generated by other member functions
	string log() const;

	/// Activates the shader program associated with this instance
	virtual bool use();

	/// Inactivates the shader program associated with this instance
	virtual void unuse();

	void setUniform(const GLchar *name, int value);
	void setUniform(const GLchar *name, unsigned int value);
	void setUniform(const GLchar *name, float value);
	void setUniform(const GLchar *name, bool value);
	void setUniform(const GLchar *name, const vec2& v);
	void setUniform(const GLchar *name, const vec3& v);
	void setUniform(const GLchar *name, const vec4& v);
	void setUniform(const GLchar *name, const mat4& m);
	void setUniform(const GLchar *name, const mat3& m);
	void setUniformArray(const GLchar *name, float *v, int elementSize, int count=1);

	/** 
	* \brief Retrieves available vertex attributes
	* \return a vector of strings containing the vertex attribute identifiers
	*/
	std::vector<std::string> getVertexAttributes();

	/** 
	* \brief Retrieves available uniform attributes
	* \return a vector of strings containing the uniform attribute identifiers
	*/
	std::vector<std::string> getUniformAttributes() const;

	/**
	* \brief Binds the specified vertex attribute to index
	* \param index to bind the attribute 'name' to
	* \param the name of the attribute
	* \return true if the vertex attribute could be bound
	*/
	bool bindAttribLocation( GLuint index, std::string& name);

	/**
	* \brief Binds the specified fragment data location to the specified variable
	* \param colorNumber to bind the attribute 'name' to
	* \param the name of the out variable in the fragment shader
	* \return true if the fragment data location could be bound
	*/
	bool bindFragDataLocation( GLuint colorNumber, std::string& name);

	/// Returns the program handle encapsulated by this instance
	GLuint getProgramHandle() const;

protected:



	/// Retrieves the info log for the specified handle (program or shader)
	string getInfoLog(GLuint handle);

	GLuint programHandle;
	string logString;
	bool linked;

};


