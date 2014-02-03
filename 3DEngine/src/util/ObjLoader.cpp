#include "stdafx.h"
#include "ObjLoader.h"
#include <fstream>
#include <regex>
#include <sstream>
#include "MeshRaw.h"
#include "../error.h"
#include "Util.h"

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::ivec3;


void ObjLoader::ParseIdx(string& s,ivec3& indices)
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

MeshRaw_ptr ObjLoader::LoadObj(istream& istr, std::string path)
{	
	int lineCount = 0;

	MeshRaw_ptr newMesh = MeshRaw::Create();
	newMesh->meshPath = path;

	string groupName;	
	string currentMtl;
	int groupFaceStartIdx = 0;

	std::string line;
	while ( istr.good() )
	{
		getline (istr,line);
		lineCount++;

		if(line.substr(0,2) == "o ")
		{
			newMesh->name = line.substr(2);
		}
		if(line.substr(0,2) == "g ")
		{			
			if(faces.size() > 0)
			{
				int groupEndIndex  = (int)faces.size() - 1;
				MeshRaw::Range range(groupFaceStartIdx, groupEndIndex);

				newMesh->addGroup(groupName,currentMtl,range);
				
				//next group
				groupFaceStartIdx = groupEndIndex + 1;
			}

			groupName = line.substr(2);

		}
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

			ParseIdx(s1,v1);
			ParseIdx(s2,v2);
			ParseIdx(s3,v3);

			Tri t(v1,v2,v3);
			faces.push_back(t);

		}
		else if(line.substr(0,7) == "mtllib ")
		{			
			std::string mtlFile =  Util::ExtractBaseFolder(currentFile) + line.substr(7);

			std::ifstream mtlFileStream(mtlFile);
			if(mtlFileStream && LoadMtllib(mtlFileStream,newMesh))
				std::cout << "Material lib loaded: " << mtlFile << std::endl;
			else
				std::cout << "Could not load mtllib: " << mtlFile << std::endl;


		}
		else if(line.substr(0,7) == "usemtl ")
		{	
			currentMtl= line.substr(7);
		}
	}

	//Complete last index group
	int groupEndIndex  = (int)faces.size() - 1;
	MeshRaw::Range range(groupFaceStartIdx, groupEndIndex);

	newMesh->addGroup(groupName,currentMtl, range);

	cout << "Mesh loaded, f=" << faces.size() <<  " , v=" << vertices.size() << " , n=" << normals.size() << " ,tex coords=" << texCoords.size() << endl;

	GetVertexArray(newMesh->vertices);
	GetIndexArray(newMesh->faces);

	if( HasTexCoords() )
		GetTexCoordArray(newMesh->texCoords);

	if( ! HasNormals() )
	{		
		Info("Normal data not present... computing normals");		
		if(! ComputeNormals() )
			Error("Could not compute normals");
	}
	GetNormalArray(newMesh->normals);

	if ( ComputeTangents() ) 
		GetTangentArray(newMesh->tangents);
	else
		Error("Could not compute tangents");

	return newMesh;
}

bool ObjLoader::LoadMtllib(std::istream& istr, MeshRaw_ptr newMesh)
{
	std::string line;

	ObjMaterial_ptr mat;

	while ( istr.good() )
	{
		getline (istr,line);
		if(line.substr(0,6) == "newmtl")
		{
			if(mat)
			{
				newMesh->materials.push_back(mat);
			}

			mat = ObjMaterial::Create(line.substr(7));
		}
		else if(mat)
		{
			if(line.substr(0,2) == "Ka")
			{
				istringstream s(line.substr(2));
				vec3 v;
				s >> mat->ambient.r; s >>  mat->ambient.g; s >>  mat->ambient.b;
			}
			else if(line.substr(0,2) == "Kd")
			{
				istringstream s(line.substr(2));
				vec3 v;
				s >> mat->diffuse.r; s >>  mat->diffuse.g; s >>  mat->diffuse.b;
			}
			else if(line.substr(0,2) == "Ks")
			{
				istringstream s(line.substr(2));
				vec3 v;
				s >> mat->specular.r; s >>  mat->specular.g; s >>  mat->specular.b;
			}
			else if(line.substr(0,1) == "d")
			{
				istringstream s(line.substr(1));
				vec3 v;
				s >> mat->opacity;
			}
			else if(line.substr(0,2) == "Ns")
			{
				istringstream s(line.substr(2));
				vec3 v;
				s >> mat->shininess;
			}
			else if(line.substr(0,5) == "illum")
			{
				istringstream s(line.substr(5));
				int illuminationModel;
				s >> illuminationModel;
				mat->specularEnabled = illuminationModel > 1;
			}
			else if(line.substr(0,6) == "map_Ka")
			{
				mat->ambientColorTexture = line.substr(7);
			}
			else if(line.substr(0,6) == "map_Kd")
			{
				mat->diffuseColorTexture = line.substr(7);
			}
			else if(line.substr(0,6) == "map_Ks")
			{
				mat->specularColorTexture = line.substr(7);
			}
			else if(line.substr(0,6) == "map_Ns")
			{
				mat->specularHightlightTexture = line.substr(7);
			}
			else if(line.substr(0,5) == "map_d")
			{
				mat->alphaMapTexture = line.substr(6);
			}
			else if(line.substr(0,8) == "map_bump")
			{
				mat->bumpMapTexture = line.substr(7);
			}
			else if(line.substr(0,4) == "bump")
			{
				mat->bumpMapTexture = line.substr(5);
			}
			else if(line.substr(0,4) == "disp")
			{
				mat->displacementMapTexture = line.substr(5);
			}
		}
	}	
	newMesh->materials.push_back(mat);
	return newMesh->materials.size() > 0;
}

bool ObjLoader::HasNormals()
{
	return normals.size() == vertices.size();
}

bool ObjLoader::HasTangents()
{
	return tangents.size() > 0;
}

bool ObjLoader::HasTexCoords()
{
	return texCoords.size() > 0;
}

///Copy vertex data
void ObjLoader::GetVertexArray (vector<float>& vertexArray)
{
	vertexArray.resize(vertices.size() * 3, 0.0f);
	memcpy(&vertexArray[0], &vertices[0],  vertices.size() * 3 * sizeof(float));
}

void ObjLoader::GetIndexArray(vector<int>& indicesArray)
{
	size_t numIndices = faces.size() * 3;

	indicesArray.reserve(numIndices);

	vector<Tri>::const_iterator triIt;
	for(triIt = faces.cbegin(); triIt != faces.cend(); triIt++)
	{
		Tri t = *triIt;
		indicesArray.push_back(t.v1.x);
		indicesArray.push_back(t.v2.x);
		indicesArray.push_back(t.v3.x);
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

void ObjLoader::GetNormalArray (vector<float>& normalArray)
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

void ObjLoader::GetTangentArray (vector<float>& tangentArray)
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

void ObjLoader::GetTexCoordArray(vector<float>& texCoordArray)
{
//Lookup directory, holds information about vertices that are already processed

	bool* processedDictionary = new bool[vertices.size()];
	memset(processedDictionary, false, vertices.size() * sizeof(bool));

	//2 floats per vertex
	texCoordArray.resize(vertices.size() * 2);

	float* txRaw = &texCoordArray[0];

	auto processTexCoords = [this,txRaw,processedDictionary](ivec3& v) 
	{
		if( ! processedDictionary[v.x] && v.y >= 0)
		{
			vec2& tx  = texCoords [v.y];
			copyVec2( &txRaw[v.x * 2], tx);
			processedDictionary[v.x] = true;
		}
	};

	for(int i = 0; i < faces.size(); i++)
	{
		Tri& t = faces[i];

		processTexCoords(t.v1);
		processTexCoords(t.v2);
		processTexCoords(t.v3);
	}
	
	delete[] processedDictionary;
}

bool ObjLoader::ComputeNormals()
{
	//see http://mrl.nyu.edu/~perlin/courses/fall2002/meshnormals.html

	//ReInit normals to zero
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


bool ObjLoader::ComputeTangents()
{
	//see http://www.terathon.com/code/tangent.html for details

	if(! (HasNormals() && HasTexCoords()) )
		return false;
	
	//ReInit normals to zero
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
		
		if(v1Idx.y >= 0 && v2Idx.y >= 0 && v3Idx.y >= 0)
		{
			vec2& tc1 = texCoords[v1Idx.y];
			vec2& tc2 = texCoords[v2Idx.y];
			vec2& tc3 = texCoords[v3Idx.y];

			vec3& v1 = vertices[v1Idx.x];
			vec3& v2 = vertices[v2Idx.x];
			vec3& v3 = vertices[v3Idx.x];
		
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

			if(!_isnan(sdir.x) && !_isnan(sdir.y) && !_isnan(sdir.z))
			{
				tangents_tmp[v1Idx.x] += sdir;
				tangents_tmp[v2Idx.x] += sdir;
				tangents_tmp[v3Idx.x] += sdir;
			}

			if(!_isnan(tdir.x) && !_isnan(tdir.y) && !_isnan(tdir.z))
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
	for(int i=0; i < vertices.size(); i++)
	{
		const vec3& n = normals[i];
		const vec3& t = tangents_tmp[i];
		const vec3& bt = bitangents[i];

		vec3 gm_tmp = t - n * glm::dot(n, t);

		if (glm::length(gm_tmp) > 0)
		{
			// Gram-Schmidt orthogonalize
			tangents[i] = vec4(glm::normalize(gm_tmp), 1.0);

			// Calculate handedness
			tangents[i].w = (glm::dot(glm::cross(n, t), bt) < 0.0f) ? -1.0f : 1.0f;
		}
		else
		{
			tangents[i] = glm::normalize(vec4(1, 1, 1, 1));
			tangents[i].w = 1;
			Warn("Tangent could not be computed");
		}
	}

	return true;

}

MeshRaw_ptr ObjLoader::LoadObjFile(const string& path)
{
	currentFile = path;

	ifstream myfile;
	myfile.open(path);

	MeshRaw_ptr newMesh;

	std::string line;
	if (myfile.is_open())
	{
		newMesh = LoadObj(myfile,path);
		myfile.close();
	}
	else 
		cerr << "Unable to open OBJ file: "  << path << endl;

	return newMesh;
}