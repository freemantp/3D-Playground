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
	static ShaderLibrary_ptr Instance();

	ShaderLibrary();

	virtual ~ShaderLibrary();

	MaterialShader_ptr ShaderLookup(const Material_cptr& material);
	
	bool AddShader(const Material_cptr& material);

	static void Reset();
	
protected:

	bool AddShader(const Material_cptr& material, const MaterialShader_ptr& shader);

	MaterialShader_ptr ShaderLookup(const std::type_info& materialType);
	bool AddShader(const std::type_info& materialType, const MaterialShader_ptr& shader);

	static ShaderLibrary_ptr instance;
	std::map<const std::type_info*, MaterialShader_ptr> materialShaderDictionary;
	
private:
	ShaderLibrary(const ShaderLibrary&);            
	ShaderLibrary& operator=(const ShaderLibrary&);
};

