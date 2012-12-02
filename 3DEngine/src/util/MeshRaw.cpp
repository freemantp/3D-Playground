#include "stdafx.h"
#include "MeshRaw.h"


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


void MeshRaw::addGroup(string& name, Range idxRange)
{
	groupRanges.push_back(idxRange);
}