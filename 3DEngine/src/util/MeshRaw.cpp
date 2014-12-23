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

bool ObjMaterial::HasTextures() const
{
	return !ambientColorTexture.empty()
		|| !diffuseColorTexture.empty()
		|| !specularColorTexture.empty()
		|| !specularHightlightTexture.empty()
		|| !alphaMapTexture.empty()
		|| !displacementMapTexture.empty();
}

MeshRaw::MeshRaw()
{
}


MeshRaw::~MeshRaw()
{
	
}

bool MeshRaw::HasNormals() const
{
	return normals.size() == vertices.size();
}

bool MeshRaw::HasTangents() const
{
	return tangents.size() == vertices.size();
}

bool MeshRaw::HasTexCoords() const
{
	return texCoords.size() == vertices.size();
}

void MeshRaw::AddGroup(std::string& name, std::string& material, Range idxRange)
{
	groupRanges.push_back(idxRange);
	groupNames.push_back(name);
	groupMaterial.push_back(material);
}

bool MeshRaw::ComputeNormals()
{
	//see http://mrl.nyu.edu/~perlin/courses/fall2002/meshnormals.html

	//ReInit normals to zero
	normals.clear();
	normals.resize(vertices.size());

	for (int i = 0; i < faces.size(); i++)
	{
		int aIdx = faces[i].v[0].x;
		int bIdx = faces[i].v[1].x;
		int cIdx = faces[i].v[2].x;
							   
		glm::vec3& a = vertices[aIdx];
		glm::vec3& b = vertices[bIdx];
		glm::vec3& c = vertices[cIdx];

		glm::vec3 e1 = b - a;
		glm::vec3 e2 = c - b;
		glm::vec3 e3 = a - c;

		glm::vec3 perpVector = glm::cross(e3, e1);
		normals[aIdx] += perpVector;

		perpVector = glm::cross(e1, e2);
		normals[bIdx] += perpVector;

		perpVector = glm::cross(e2, e3);
		normals[cIdx] += perpVector;

		//Update vertex indices
		faces[i].v[0].z = aIdx;
		faces[i].v[1].z = bIdx;
		faces[i].v[2].z = cIdx;
	}

	//Normalize normals (unit-length)
	for (int i = 0; i < normals.size(); i++) {
		normals[i] = glm::normalize(normals[i]);
	}

	return true;
}

bool MeshRaw::ComputeTangents()
{
	//see http://www.terathon.com/code/tangent.html for details

	if (!(HasNormals() && HasTexCoords()))
		return false;

	if (texCoords.size() != vertices.size())
	{
		Error("ObjLoader: #tex coords does not equal #vertices");
		return false;
	}

	//ReInit normals to zero
	tangents.clear();
	tangents.resize(vertices.size());

	std::vector<glm::vec3> tangents_tmp(vertices.size());
	tangents_tmp.resize(vertices.size());

	std::vector<glm::vec3> bitangents(vertices.size());
	bitangents.resize(vertices.size());

	//Accumulate tangent directions
	for (int i = 0; i < faces.size(); i++)
	{
		glm::ivec3 v1Idx = faces[i].v[0];
		glm::ivec3 v2Idx = faces[i].v[1];
		glm::ivec3 v3Idx = faces[i].v[2];

		if (v1Idx.y >= 0 && v2Idx.y >= 0 && v3Idx.y >= 0)
		{
			glm::vec2& tc1 = texCoords[v1Idx.y];
			glm::vec2& tc2 = texCoords[v2Idx.y];
			glm::vec2& tc3 = texCoords[v3Idx.y];

			glm::vec3& v1 = vertices[v1Idx.x];
			glm::vec3& v2 = vertices[v2Idx.x];
			glm::vec3& v3 = vertices[v3Idx.x];

			//difference vectors
			glm::vec3 q1 = v2 - v1;
			glm::vec3 q2 = v3 - v1;

			//difference tex coords
			glm::vec2 dTc1 = tc2 - tc1;
			glm::vec2 dTc2 = tc3 - tc1;

			//Solve for tangents and bitangents
			glm::mat3x2 Q = glm::transpose(glm::mat2x3(q1, q2));
			glm::mat2 TX_1 = glm::inverse(glm::mat2(dTc1, dTc2));
			glm::mat2x3 R = glm::transpose(TX_1 * Q);

			glm::vec3 sdir = R[0];
			glm::vec3 tdir = R[1];

			if (!_isnan(sdir.x) && !_isnan(sdir.y) && !_isnan(sdir.z))
			{
				tangents_tmp[v1Idx.x] += sdir;
				tangents_tmp[v2Idx.x] += sdir;
				tangents_tmp[v3Idx.x] += sdir;
			}

			if (!_isnan(tdir.x) && !_isnan(tdir.y) && !_isnan(tdir.z))
			{
				bitangents[v1Idx.x] += tdir;
				bitangents[v2Idx.x] += tdir;
				bitangents[v3Idx.x] += tdir;
			}
		}
		else
		{
			tangents.clear();
			bitangents.clear();
			return false;
		}
	}

	//Orthogonalize and normalize
	for (int i = 0; i < vertices.size(); i++)
	{
		const glm::vec3& n = normals[i];
		const glm::vec3& t = tangents_tmp[i];
		const glm::vec3& bt = bitangents[i];

		glm::vec3 gm_tmp = t - n * glm::dot(n, t);

		if (glm::length(gm_tmp) > 0)
		{
			// Gram-Schmidt orthogonalization
			tangents[i] = glm::vec4(glm::normalize(gm_tmp), 1.0);

			// Calculate handedness
			tangents[i].w = (glm::dot(glm::cross(n, t), bt) < 0.0f) ? -1.0f : 1.0f;
		}
		else
		{
			tangents[i] = glm::normalize(glm::vec4(1, 1, 1, 1));
			tangents[i].w = 1;
			Warn("Tangent could not be computed");
		}
	}

	return true;
}

void MeshRaw::ConvertIndices()
{
	indices.clear();
	indices.reserve(faces.size() * 3);
	for (auto t : faces)
	{
		indices.push_back(t.v[0].x);
		indices.push_back(t.v[1].x);
		indices.push_back(t.v[2].x);
	}
}