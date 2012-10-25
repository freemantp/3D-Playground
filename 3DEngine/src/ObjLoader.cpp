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

void ObjLoader::loadObj(istream& istr)
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
			s >> tc.x; s >> tc.y;
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

			triangles.push_back(Tri(v1,v2,v3));

		}
		
	}
	cout << "Mesh loaded, f=" << triangles.size() <<  " , v=" << vertices.size() / 3 << " , n=" << normals.size() << " ,tex coords=" << texCoords.size() << endl;
	
}

bool ObjLoader::hasNormals()
{
	return normals.size() > 0;
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
	indexArray.reserve(triangles.size() * 3);

	vector<Tri>::const_iterator triIt;
	for(triIt = triangles.cbegin(); triIt != triangles.cend(); triIt++)
	{
		Tri t = *triIt;
		indexArray.push_back(t.v1.x);
		indexArray.push_back(t.v2.x);
		indexArray.push_back(t.v3.x);
	}
}

inline void copyVec3(float* target, const vec3& v)
{
	memcpy( target, &v.x, 3 * sizeof(float));	
}

void ObjLoader::getNormalArray (vector<float>& normalArray)
{
	//Lookup directory, holds information about vertices that are already processed
	bool* processedDictionary = new bool[vertices.size()];
	memset(processedDictionary, false, vertices.size() * sizeof(bool));

	//3 floats per vertex
	normalArray.resize(vertices.size() * 3);

	float* normalRaw = &normalArray[0];

	for(int i = 0; i < triangles.size(); i++)
	{
		Tri& t = triangles[i];

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

void ObjLoader::getTexCoordArray(vector<float>& texCoordArray)
{
	
}

bool ObjLoader::computeNormals()
{
	//see http://mrl.nyu.edu/~perlin/courses/fall2002/meshnormals.html

	//Reinit normals to zero
	normals.clear();
	normals.resize(vertices.size());

	for(int i = 0; i < triangles.size(); i++)
	{		
		int aIdx = triangles[i].v1.x;
		int bIdx = triangles[i].v2.x;
		int cIdx = triangles[i].v3.x;
		
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
		triangles[i].v1.z = aIdx;
		triangles[i].v2.z = bIdx;
		triangles[i].v3.z = cIdx;
	}
		
	//Normalize normals (unit-length)
	for(int i = 0; i < normals.size(); i++) {
		normals[i] = glm::normalize(normals[i]);
	}

	return true;
}

void ObjLoader::loadObjFile(const string& path)
{
	ifstream myfile;
	myfile.open(path);

	std::string line;
	if (myfile.is_open())
	{
		loadObj(myfile);
		myfile.close();
	}
	else 
		cerr << "Unable to open file" << endl;; 
}