#pragma once

/*
	GETSET(int,MyInt)   
 
	GETSET(float,MyFloat)
 
	GETSET(string,MyString)
*/

#define GETSET(type, var) \
private: \
   type m_##var; \
public: \
   type Get##var() \
   {\
      return  m_##var; \
   }\
   void Set##var(type val) \
   {\
       m_##var = val; \
   }


