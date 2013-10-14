#include "stdafx.h"

#include "MeshRaw.h"


ObjMaterial_ptr ObjMaterial::Create(std::string name)
{
	return ObjMaterial_ptr(new ObjMaterial(name));
}

ObjMaterial::ObjMaterial(std::string theName)
	: opacity(1)
	, specularEnabled(true)
	, name(theName)
{

}

MeshRaw::MeshRaw(void)
{
}


MeshRaw::~MeshRaw(void)
{
	
}

bool MeshRaw::hasNormals()
{
	return normals.size() > 0;
}

bool MeshRaw::hasTangents()
{
	return tangents.size() > 0;
}

bool MeshRaw::hasTexCoords()
{
	return texCoords.size() > 0;
}

void MeshRaw::addGroup(string& name, string& material, Range idxRange)
{
	groupRanges.push_back(idxRange);
	groupNames.push_back(name);
	groupMaterial.push_back(material);
}