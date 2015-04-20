#include "stdafx.h"

#include "ObjLoader.h"
#include "RawMesh.h"
#include "Util.h"

#include "../error.h"

#include <fstream>
#include <regex>
#include <sstream>
#include <array>
#include <deque>

using glm::vec2;
using glm::vec3;
using glm::ivec3;


IndexedRawMesh_ptr ObjLoader::LoadObj(std::istream& istr, std::string path)
{	
	int lineCount = 0;

	IndexedRawMesh_ptr newMesh = IndexedRawMesh::Create();
	newMesh->meshPath = path;

	std::string groupName;	
	std::string currentMtl;
	int groupFaceStartIdx = 0;

	auto create_group = [&](std::string& groupName)
	{
		if (newMesh->faces.size() > 0)
		{
			int groupEndIndex = static_cast<int>(newMesh->faces.size() - 1);
			IndexedRawMesh::Range range(groupFaceStartIdx, groupEndIndex);

			newMesh->AddGroup(groupName, currentMtl, range);

			//next group
			groupFaceStartIdx = groupEndIndex + 1;
		}
	};

	std::string line;
	while ( istr.good() )
	{
		std::getline(istr,line);
		auto num_chars = line.size();
		lineCount++;

		if (line[0] == 'o' && num_chars > 2)
		{
			newMesh->name = std::string(line.begin() + 2, line.end());

			if (newMesh->faces.size() > 0)
			{
				create_group(newMesh->name);
			}
		}
		else if (line[0] == 'g' && num_chars > 2)
		{			
			groupName = std::string(line.begin() + 2, line.end());

			if (newMesh->faces.size() > 0)
			{
				create_group(groupName);
			}
		}
		else if (line[0] == 'v' && line[1] == ' ')
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
			v.y = 1.f - std::strtof(end_p1, nullptr);
			newMesh->texCoords.push_back(v);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			std::istringstream str_stream(line);
			str_stream.ignore(2);

			std::deque<glm::ivec3> vtx_queue;

			bool vtx_read = false;
			glm::ivec3 first_vtx;
			glm::ivec3 current_vtx(-1,-1,-1);
			int comp = 0;

			while (!str_stream.eof())
			{
				str_stream >> current_vtx[comp];
				current_vtx[comp]--;

				if (str_stream.peek() == '/')
				{
					str_stream.ignore();
					int skip_comp = 1;

					if (str_stream.peek() == '/')
					{
						str_stream.ignore();
						skip_comp++;
					}

					comp += skip_comp;
				}
				else if (str_stream.eof() || str_stream.peek() == ' ')
				{
					str_stream.ignore();
					comp = 0;

					if (!vtx_read)
					{
						first_vtx = current_vtx;
						vtx_read = true;
					}
					else
					{
						vtx_queue.push_back(current_vtx);						
					}

					current_vtx = glm::ivec3(-1, -1, -1);

					if (vtx_queue.size() == 2)
					{
						Tri triangle;
						triangle.Set(0, first_vtx);
						triangle.Set(1, vtx_queue[0]);
						triangle.Set(2, vtx_queue[1]);
						vtx_queue.pop_front();

						newMesh->faces.push_back(triangle);
					}
				}
				else
				{
					str_stream.ignore();
				}
			}
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
	create_group(groupName);

	if (newMesh->name.empty())
		newMesh->name = Util::ExtractFileName(newMesh->meshPath);

	std::cout << "Wavefront OBJ mesh loaded, f=" 
		<< newMesh->faces.size() 
		<< ", v=" << newMesh->vertices.size() 
		<< ", n=" << newMesh->normals.size() 
		<< ", tex coords=" 
		<< newMesh->texCoords.size() 
		<< std::endl;

	return newMesh;
}

bool ObjLoader::LoadMtllib(std::istream& istr, IndexedRawMesh_ptr newMesh)
{
	std::string line;

	WavefrontObjMaterial_ptr mat;

	while ( istr.good() )
	{
		getline (istr,line);
		if(line.substr(0,6) == "newmtl")
		{
			if(mat)
			{
				newMesh->materials.push_back(mat);
			}

			mat = WavefrontObjMaterial::Create(line.substr(7));
		}
		else if(mat)
		{
			if(line.substr(0,2) == "Ka")
			{
				std::istringstream s(line.substr(2));
				vec3 v;
				s >> mat->ambient.r; s >>  mat->ambient.g; s >>  mat->ambient.b;
			}
			else if(line.substr(0,2) == "Kd")
			{
				std::istringstream s(line.substr(2));
				vec3 v;
				s >> mat->diffuse.r; s >>  mat->diffuse.g; s >>  mat->diffuse.b;
			}
			else if(line.substr(0,2) == "Ks")
			{
				std::istringstream s(line.substr(2));
				vec3 v;
				s >> mat->specular.r; s >>  mat->specular.g; s >>  mat->specular.b;
			}
			else if(line.substr(0,1) == "d")
			{
				std::istringstream s(line.substr(1));
				vec3 v;
				s >> mat->opacity;
			}
			else if(line.substr(0,2) == "Ns")
			{
				std::istringstream s(line.substr(2));
				vec3 v;
				s >> mat->shininess;
			}
			else if(line.substr(0,5) == "illum")
			{
				std::istringstream s(line.substr(5));
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

IndexedRawMesh_ptr ObjLoader::LoadObjFile(const std::string& path)
{
	currentFile = path;

	std::ifstream myfile;
	myfile.open(path);

	IndexedRawMesh_ptr newMesh;

	std::string line;
	if (myfile.is_open())
	{
		newMesh = LoadObj(myfile,path);
		myfile.close();
	}
	else 
		std::cerr << "Unable to open OBJ file: "  << path << std::endl;

	return newMesh;
}