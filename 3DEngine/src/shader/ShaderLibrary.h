#pragma once

#include "../util/SharedPointer.h"
#include "../materials/Material.h"

#include <map> 
#include <typeinfo> 

SHARED_PTR_CLASS_DECL(ShaderLibrary)
SHARED_PTR_CLASS_DECL(MaterialShader)

class ShaderLibrary
{
public:	
	static ShaderLibrary_ptr GetInstance();

	MaterialShader_ptr ShaderLookup(Material_cptr material);
	
	bool AddShader(Material_cptr material);
	
protected:

	virtual ~ShaderLibrary();

	bool AddShader(Material_cptr material, MaterialShader_ptr shader);

	MaterialShader_ptr ShaderLookup(const std::type_info& materialType);
	bool AddShader(const std::type_info& materialType, MaterialShader_ptr shader);

	static ShaderLibrary_ptr instance;
	std::map<const std::type_info*, MaterialShader_ptr> materialShaderDictionary;
	
private:
	ShaderLibrary();	
	ShaderLibrary(const ShaderLibrary&);            
	ShaderLibrary& operator=(const ShaderLibrary&);
};

