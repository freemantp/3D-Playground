#include "stdafx.h"
#include "ObjLoader.h"
#include <fstream>
#include <regex>
#include <sstream>
#include <glm/core/func_geometric.hpp>

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::ivec3;

void ObjLoader::parseIdx(string& s,ivec3& indices)
{
	string::const_iterator sit;
	stringstream ss;

	int idxPtr = 0;
	int number;
	bool lastWasSlash = false;

	for(sit = s.cbegin(); sit != s.cend(); sit++)
	{
		char c = *sit;
		
		if(c == '/') {
			if( lastWasSlash)			
				indices[idxPtr++] = -1;
			else {
				ss >> number;
				indices[idxPtr++] = number - 1;
				ss.str("");
				ss.clear();
			}				
	
			lastWasSlash = true;
		}
		else {
			ss << c;
			lastWasSlash = false;
		}
	}

	ss >> number;
	indices[idxPtr++] = number - 1;
}

bool ObjLoader::loadObj(istream& istr)
{	
	int lineCount = 0;

	std::string line;
	while ( istr.good() )
	{
		getline (istr,line);
		lineCount++;

		if(line.substr(0,2) == "v ")
		{
			istringstream s(line.substr(2));
			vec3 v;
			s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(v);

		}
		else if(line.substr(0,3) == "vn ")
		{
			istringstream s(line.substr(3));
			glm::vec3 n; 
			s >> n.x; s >> n.y; s >> n.z;
			normals.push_back(n);
		}
		else if(line.substr(0,3) == "vt ")
		{
			istringstream s(line.substr(3));
			glm::vec2 tc; 
			s >> tc.x;  s >> tc.y;

			//Invert y-axis
			 tc.y =  1 - tc.y;

			texCoords.push_back(tc);
		}
		else if(line.substr(0,2) == "f ")
		{			
			ivec3 v1,v2,v3;
			string s1,s2,s3;
			string sub = line.substr(2);
			istringstream data(sub);
			data >> s1; data >> s2; data >> s3;

			parseIdx(s1,v1);
			parseIdx(s2,v2);
			parseIdx(s3,v3);

			faces.push_back(Tri(v1,v2,v3));

		}
		
	}
	cout << "Mesh loaded, f=" << faces.size() <<  " , v=" << vertices.size() / 3 << " , n=" << normals.size() << " ,tex coords=" << texCoords.size() << endl;
	return true;
}

bool ObjLoader::hasNormals()
{
	return normals.size() > 0;
}

bool ObjLoader::hasTangents()
{
	return tangents.size() > 0;
}

bool ObjLoader::hasTexCoords()
{
	return texCoords.size() > 0;
}

void ObjLoader::getVertexArray (vector<float>& vertexArray)
{
	vertexArray.resize(vertices.size() * 3, 0.0f);
	
	//Copy vertex data
	for(int i = 0; i < vertices.size(); i++)
	{
		memcpy(&vertexArray[i*3], &vertices[i], 3 * sizeof(float));
	}

	//memcpy(&vertexArray[0], &vertices[0],  vertices.size() * 3 * sizeof(float));

}

void ObjLoader::getIndexArray(vector<int>& indexArray)
{
	indexArray.clear();
	indexArray.reserve(faces.size() * 3);

	vector<Tri>::const_iterator triIt;
	for(triIt = faces.cbegin(); triIt != faces.cend(); triIt++)
	{
		Tri t = *triIt;
		indexArray.push_back(t.v1.x);
		indexArray.push_back(t.v2.x);
		indexArray.push_back(t.v3.x);
	}
}

void ObjLoader::copyVec4(float* target, const vec4& v)
{
	memcpy( target, &v.x, 4 * sizeof(float));	
}


void ObjLoader::copyVec3(float* target, const vec3& v)
{
	memcpy( target, &v.x, 3 * sizeof(float));	
}

void ObjLoader::copyVec2(float* target, const vec2& v)
{
	memcpy( target, &v.x, 2 * sizeof(float));	
}

void ObjLoader::getNormalArray (vector<float>& normalArray)
{
	//Lookup directory, holds information about vertices that are already processed
	bool* processedDictionary = new bool[vertices.size()];
	memset(processedDictionary, false, vertices.size() * sizeof(bool));

	//3 floats per vertex
	normalArray.resize(vertices.size() * 3);

	float* normalRaw = &normalArray[0];

	for(int i = 0; i < faces.size(); i++)
	{
		Tri& t = faces[i];

		if( ! processedDictionary[t.v1.x])
		{
			vec3& n  = normals [t.v1.z];
			copyVec3( &normalRaw[t.v1.x * 3], n);
			processedDictionary[t.v1.x] = true;
		}
		if( ! processedDictionary[t.v2.x])
		{
			vec3& n  = normals  [t.v2.z];
			copyVec3( &normalRaw[t.v2.x * 3], n);
			processedDictionary[t.v2.x] = true;
		}
		if( ! processedDictionary[t.v3.x])
		{
			vec3& n  = normals  [t.v3.z];
			copyVec3( &normalRaw[t.v3.x * 3], n);
			processedDictionary[t.v3.x] = true;
		}
	}
	
	delete[] processedDictionary;
}

void ObjLoader::getTangentArray (vector<float>& tangentArray)
{
	//Lookup directory, holds information about vertices that are already processed
	bool* processedDictionary = new bool[vertices.size()];
	memset(processedDictionary, false, vertices.size() * sizeof(bool));

	//3 floats per vertex
	tangentArray.resize(vertices.size() * 4);

	float* tangentRaw = &tangentArray[0];

	for(int i = 0; i < faces.size(); i++)
	{
		Tri& t = faces[i];

		if( ! processedDictionary[t.v1.x])
		{
			vec4& tn  = tangents [t.v1.z];
			copyVec4( &tangentRaw[t.v1.x * 4], tn);
			processedDictionary[t.v1.x] = true;
		}
		if( ! processedDictionary[t.v2.x])
		{
			vec4& tn  = tangents  [t.v2.z];
			copyVec4( &tangentRaw[t.v2.x * 4], tn);
			processedDictionary[t.v2.x] = true;
		}
		if( ! processedDictionary[t.v3.x])
		{
			vec4& tn  = tangents  [t.v3.z];
			copyVec4( &tangentRaw[t.v3.x * 4], tn);
			processedDictionary[t.v3.x] = true;
		}
	}
	
	delete[] processedDictionary;
}

void ObjLoader::getTexCoordArray(vector<float>& texCoordArray)
{
//Lookup directory, holds information about vertices that are already processed

	bool* processedDictionary = new bool[vertices.size()];
	memset(processedDictionary, false, vertices.size() * sizeof(bool));

	//2 floats per vertex
	texCoordArray.resize(vertices.size() * 2);

	float* txRaw = &texCoordArray[0];

	for(int i = 0; i < faces.size(); i++)
	{
		Tri& t = faces[i];

		if( ! processedDictionary[t.v1.x])
		{
			vec2& tx  = texCoords [t.v1.y];
			copyVec2( &txRaw[t.v1.x * 2], tx);
			processedDictionary[t.v1.x] = true;
		}
		if( ! processedDictionary[t.v2.x])
		{
			vec2& tx  = texCoords  [t.v2.y];
			copyVec2( &txRaw[t.v2.x * 2], tx);
			processedDictionary[t.v2.x] = true;
		}
		if( ! processedDictionary[t.v3.x])
		{
			vec2& tx  = texCoords  [t.v3.y];
			copyVec2( &txRaw[t.v3.x * 2], tx);
			processedDictionary[t.v3.x] = true;
		}
	}
	
	delete[] processedDictionary;
}

bool ObjLoader::computeNormals()
{
	//see http://mrl.nyu.edu/~perlin/courses/fall2002/meshnormals.html

	//Reinit normals to zero
	normals.clear();
	normals.resize(vertices.size());

	for(int i = 0; i < faces.size(); i++)
	{		
		int aIdx = faces[i].v1.x;
		int bIdx = faces[i].v2.x;
		int cIdx = faces[i].v3.x;
		
		vec3& a = vertices[aIdx];
		vec3& b = vertices[bIdx];
		vec3& c = vertices[cIdx];
		
		vec3 e1 = b-a;
		vec3 e2 = c-b;
		vec3 e3 = a-c;

		vec3 perpVector = glm::cross(e3,e1);
		normals[aIdx] += perpVector;

		perpVector = glm::cross(e1,e2);	
		normals[bIdx] += perpVector;

		perpVector = glm::cross(e2,e3);	
		normals[cIdx] += perpVector;

		//Update vertex indices
		faces[i].v1.z = aIdx;
		faces[i].v2.z = bIdx;
		faces[i].v3.z = cIdx;
	}
		
	//Normalize normals (unit-length)
	for(int i = 0; i < normals.size(); i++) {
		normals[i] = glm::normalize(normals[i]);
	}

	return true;
}


bool ObjLoader::computeTangents()
{
	//see http://www.terathon.com/code/tangent.html for details

	if(! (hasNormals() && hasTexCoords()) )
		return false;
	
	//Reinit normals to zero
	tangents.clear();
	tangents.resize(vertices.size());

	vector<vec3> tangents_tmp(vertices.size());
	tangents_tmp.resize(vertices.size());

	vector<vec3> bitangents(vertices.size());
	bitangents.resize(vertices.size());
	
	//Accumulate tangent directions
	for(int i = 0; i < faces.size(); i++)
	{		
		ivec3 v1Idx = faces[i].v1;
		ivec3 v2Idx = faces[i].v2;
		ivec3 v3Idx = faces[i].v3;
		
		vec3& v1 = vertices[v1Idx.x];
		vec3& v2 = vertices[v2Idx.x];
		vec3& v3 = vertices[v3Idx.x];

		vec2& tc1 = texCoords[v1Idx.y];
		vec2& tc2 = texCoords[v2Idx.y];
		vec2& tc3 = texCoords[v3Idx.y];
		
		//difference vectors
		vec3 q1 = v2-v1;
		vec3 q2 = v3-v1;

		//difference tex coords
		vec2 dTc1 = tc2 - tc1;
		vec2 dTc2 = tc3 - tc1;
		
		//Sove for tangents and bitangents
		glm::mat3x2 Q = glm::transpose(glm::mat2x3(q1,q2));
		glm::mat2 TX_1= glm::inverse(glm::mat2(dTc1,dTc2));
		glm::mat2x3 R = glm::transpose(TX_1 * Q);

		vec3 sdir = R[0];
		vec3 tdir = R[1];

		tangents_tmp[v1Idx.x] += sdir;
		tangents_tmp[v2Idx.x] += sdir;
		tangents_tmp[v3Idx.x] += sdir;

		bitangents[v1Idx.x] += tdir;
		bitangents[v2Idx.x] += tdir;
		bitangents[v3Idx.x] += tdir;
	}

	//Orthogonalize and normalize
	for(int i=0; i < vertices.size(); i++)
	{
		const vec3& n = normals[i];
		const vec3& t = tangents_tmp[i];
		const vec3& bt = bitangents[i];

		// Gram-Schmidt orthogonalize
		tangents[i] = vec4(glm::normalize(t - n * glm::dot(n,t) ), 1.0);

		// Calculate handedness
        tangents[i].w = (glm::dot(glm::cross(n, t), bt) < 0.0f) ? -1.0f : 1.0f;
	}

	return true;

}

bool ObjLoader::loadObjFile(const string& path)
{
	ifstream myfile;
	myfile.open(path);

	bool success = true;

	std::string line;
	if (myfile.is_open())
	{
		success = loadObj(myfile);
		myfile.close();
	}
	else 
	{
		cerr << "Unable to open OBJ file: "  << path << endl;
		success = false;
	}

	return success;
}