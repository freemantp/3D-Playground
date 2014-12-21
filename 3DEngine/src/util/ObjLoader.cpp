#include "stdafx.h"

#include "ObjLoader.h"
#include "MeshRaw.h"
#include "Util.h"

#include "../error.h"

#include <fstream>
#include <regex>
#include <sstream>
#include <array>

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
			if (newMesh->faces.size() > 0)
			{
				int groupEndIndex = static_cast<int>(newMesh->faces.size() - 1);
				MeshRaw::Range range(groupFaceStartIdx, groupEndIndex);

				newMesh->AddGroup(groupName,currentMtl,range);
				
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
			newMesh->vertices.push_back(v);

		}
		else if(line.substr(0,3) == "vn ")
		{
			istringstream s(line.substr(3));
			glm::vec3 n; 
			s >> n.x; s >> n.y; s >> n.z;
			newMesh->normals.push_back(n);
		}
		else if(line.substr(0,3) == "vt ")
		{
			istringstream s(line.substr(3));
			glm::vec2 tc; 
			s >> tc.x;  s >> tc.y;

			//Invert y-axis
			 tc.y =  1 - tc.y;

			 newMesh->texCoords.push_back(tc);
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
			newMesh->faces.push_back(t);

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
	int groupEndIndex = static_cast<int>(newMesh->faces.size() - 1);
	MeshRaw::Range range(groupFaceStartIdx, groupEndIndex);

	newMesh->AddGroup(groupName,currentMtl, range);

	cout << "Mesh loaded, f=" << newMesh->faces.size() << ", v=" << newMesh->vertices.size() << ", n=" << newMesh->normals.size() << ", tex coords=" << newMesh->texCoords.size() << endl;

	newMesh->ConvertIndices();

	if (!newMesh->HasNormals())
	{		
		Info("Normal data not present... computing normals");		
		if (!newMesh->ComputeNormals())
			Error("Could not compute normals");
	}

	if (newMesh->HasNormals() && newMesh->HasTexCoords())
	{
		if (!newMesh->ComputeTangents())
			Error("Could not compute tangents");
	}

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