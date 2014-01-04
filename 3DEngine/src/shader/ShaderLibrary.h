#pragma once

#include "../util/SharedPointer.h"
#include "../materials/Material.h"

#include <map> 
#include <typeinfo> 

SHARED_PTR_CLASS_DECL(ShaderLibrary)
SHARED_PTR_CLASS_DECL(ShaderBase)

class ShaderLibrary
{
public:	
	static ShaderLibrary_cptr GetInstance();
		
	virtual ~ShaderLibrary();

	ShaderBase_ptr GetShader(Material_cptr material);
	
	bool AddShader(Material_cptr material, ShaderBase_ptr shader);
	


protected:

	ShaderBase_ptr GetShader(const std::type_info& materialType);
	bool AddShader(const std::type_info& materialType, ShaderBase_ptr shader);

	static ShaderLibrary_ptr instance;
	std::map<const std::type_info*, ShaderBase_ptr> materialShaderDictionary;
	
private:
	ShaderLibrary();	
	ShaderLibrary(const ShaderLibrary&);            
	ShaderLibrary& operator=(const ShaderLibrary&);
};

