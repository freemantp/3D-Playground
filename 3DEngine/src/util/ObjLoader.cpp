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
		std::getline(istr,line);
		auto num_chars = line.size();
		lineCount++;

		if (line[0] == 'o' && num_chars > 2)
		{
			newMesh->name = std::string(line.begin() + 2, line.end());
		}
		if (line[0] == 'g' && num_chars > 2)
		{			
			groupName = std::string(line.begin() + 2, line.end());

			if (newMesh->faces.size() > 0)
			{
				int groupEndIndex = static_cast<int>(newMesh->faces.size() - 1);
				MeshRaw::Range range(groupFaceStartIdx, groupEndIndex);

				newMesh->AddGroup(groupName,currentMtl,range);
				
				//next group
				groupFaceStartIdx = groupEndIndex + 1;
			}
		}
		if (line[0] == 'v' && line[1] == ' ')
		{
			glm::vec3 v;
			char* end_p1, *end_p2;
			v.x = std::strtof(&line[2], &end_p1);
			v.y = std::strtof(end_p1, &end_p2);
			v.z = std::strtof(end_p2, nullptr);
			newMesh->vertices.push_back(v);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			glm::vec3 v;
			char* end_p1, *end_p2;
			v.x = std::strtof(&line[3], &end_p1);
			v.y = std::strtof(end_p1, &end_p2);
			v.z = std::strtof(end_p2, nullptr);
			newMesh->normals.push_back(v);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			glm::vec2 v;
			char* end_p1;
			v.x = std::strtof(&line[3], &end_p1);
			v.y = std::strtof(end_p1, nullptr);
			newMesh->texCoords.push_back(v);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{			
			istringstream str_stream(line);
			str_stream.ignore(2);

			Tri tri;

			int vertex = 0;
			int comp = 0;

			while (!str_stream.eof())
			{
				str_stream >> tri.v[vertex][comp];
				tri.v[vertex][comp]--;

				if (str_stream.peek() == '/')
				{
					str_stream.ignore();
					int ignore = 1;
					
					if (str_stream.peek() == '/')
					{
						str_stream.ignore();
						ignore++;
					}
						
					comp += ignore;
				}
				else if (str_stream.peek() == ' ')
				{
					str_stream.ignore();
					vertex++;
					comp = 0;
				}
			}

			newMesh->faces.push_back(tri);

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