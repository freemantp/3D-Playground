#pragma once

#include "core/gl.h"

#include <iostream>
#include <string>
#include <sstream>

//#define Beep 1

#ifdef Beep
#include "util/Util.h"
#endif

inline void Info(const char* msg) 
{
	std::cout << "INFO:  " << msg << std::endl;
}

inline void Info(const std::string& msg) 
{
	std::cout << "INFO:  " << msg << std::endl;
}

inline void Debug(const std::string& msg)
{
	std::cout << "DEBUG:  " << msg << std::endl;
}

inline void Error(const char* errorMsg) 
{
	std::cerr << "ERROR: " << errorMsg << std::endl;
	#ifdef Beep 
	Util::Beep();
	#endif
}

inline void Error(const std::string& errorMsg) 
{
	std::cerr << "ERROR: " << errorMsg << std::endl;
	#ifdef Beep 
	Util::Beep();
	#endif
}

inline void CheckOpenGlError(std::string file, int line) 
{
#if _DEBUG 
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		#ifdef Beep 
		Util::Beep();
		#endif
		std::cerr << "ERROR [OpenGL]: " << err << " on "<<  file << ":" << line << std::endl;
	}
#endif
}
//macro with file and line nr
#define OpenGLErrorCheck() CheckOpenGlError(__FILE__,__LINE__)


inline void Error(const std::string& errorMsg, int line) 
{
	#ifdef Beep 
	Util::Beep();
	#endif
	std::stringstream err;
	err << errorMsg << ", line " << line;
	std::cerr << "ERROR: " << err.str() << std::endl;
}

inline void Warn(const char* errorMsg) 
{
	#ifdef Beep 
	Util::Beep();
	#endif
	std::cout << "WARNING: " << errorMsg << std::endl;
}

inline void Warn(const std::string& errorMsg) 
{
	#ifdef Beep 
	Util::Beep();
	#endif
	std::cout << "WARNING: " << errorMsg << std::endl;
}

inline void Warn(const std::string& warnMsg, int line) 
{
	#ifdef Beep 
	Util::Beep();
	#endif
	std::stringstream warn;
	warn << warnMsg << ", line " << line;
	std::cout << "WARNING: " << warn.str() << std::endl;
}