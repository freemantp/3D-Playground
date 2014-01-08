#include "stdafx.h"
#include "Mesh.h"
#include "../scene/Scene.h"
#include "../shader/ShaderLibrary.h"
#include "../shader/ShaderBase.h"
#include "../shader/MaterialShader.h"
#include "../texture/Texture.h"
#include "../util/Util.h"
#include "../util/MeshRaw.h"
#include "../materials/Material.h"

#include <set>

using namespace GLSLShader;

Mesh_ptr Mesh::Create(MeshRaw_ptr mesh)
{
	return Mesh_ptr(new Mesh(mesh));
}

Mesh::Mesh() 
	: initialized(false)
	, Shape() 
{
	Init();
}

Mesh::Mesh(MeshRaw_ptr rawMesh)
	: initialized(false)
	, Shape() 
{
	Init();
	InitFromRawMesh(rawMesh);
	
}

Mesh::~Mesh()
{
	delete[] bufferObjects;
	delete[] indexBufferObjects;
	delete[] vAttribData;
}

void Mesh::InitFromRawMesh(MeshRaw_ptr rawMesh)
{
	if(rawMesh)
	{
		SetPositions(rawMesh->vertices,rawMesh->faces, &rawMesh->groupRanges);

		std::vector<PhongMaterial_ptr> meshMaterials;

		// Mesh has material descriptors
		for (auto mat : rawMesh->materials)
		{
			PhongMaterial_ptr pMaterial;
			if (mat->HasTextures())
			{
				TextureMaterial_ptr texMat = TextureMaterial::Create();
				pMaterial = texMat;

				if (!mat->bumpMapTexture.empty())
				{
					texMat->bumpTexture = Texture::Create(Util::ExtractBaseFolder(rawMesh->meshPath) + mat->bumpMapTexture);
					texMat->bumpIsNormalMap = true;
				}
				if (!mat->diffuseColorTexture.empty())
				{
					texMat->albedoTexture = Texture::Create(Util::ExtractBaseFolder(rawMesh->meshPath) + mat->diffuseColorTexture);
				}
				if (!mat->specularColorTexture.empty())
				{
					texMat->specularTexture = Texture::Create(Util::ExtractBaseFolder(rawMesh->meshPath) + mat->specularColorTexture);
				}
			}
			else
			{
				pMaterial = PhongMaterial::Create();
			}

			pMaterial->SetName(mat->name);
			pMaterial->ambientReflection = mat->ambient;
			pMaterial->diffuseReflection = mat->diffuse;
			pMaterial->glossyReflection = mat->specular;
			pMaterial->shininess = static_cast<int>(mat->shininess);
			pMaterial->opacity = mat->opacity;

			meshMaterials.push_back(pMaterial);
		}

		// TODO: Refactor this (ugly, inefficient hack)
		for(auto matName : rawMesh->groupMaterial)
		{
			bool found = false;

			for (Material_ptr mat : meshMaterials)
			{
				std::string mn = mat->GetName();

				if(matName == mn)
				{
					materialsNew.push_back(mat);
					found = true;
				}
			}
		}

		if( rawMesh->HasTexCoords() )
			SetTextureCoordinates(rawMesh->texCoords);
	
		if( rawMesh->HasNormals() )
			SetNormals(rawMesh->normals);
		else	
			Warn("Normal data not present!");		

		if ( rawMesh->HasTangents() ) 
			SetTangents(rawMesh->tangents);
		else
			Warn("Tangent data not present!");
	}
}

bool Mesh::MapVertexAttribute(VertexAttribute attrib, GLuint channel) const
{
	if ( attrib == Index) //Indices are NOT a vertex attribute!
		return false;

	GLuint currentChannel = vAttribData[attrib].channel;

	if (currentChannel != channel && GL_TRUE == glIsBuffer(bufferObjects[attrib]))
	{		
		//bind array & buffer
		glBindVertexArray(vaoHandle);	
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[attrib]);

		glDisableVertexAttribArray(currentChannel);

		glEnableVertexAttribArray(channel);

		vAttribData[attrib].channel = channel;
		SetAttribPointer(attrib);

		//unbind array & buffer
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);
	}
	
	return true;
}

void Mesh::SetAttribPointer(const VertexAttribute& attrib) const
{
	GLuint channel = vAttribData[attrib].channel;
	GLint size = vAttribData[attrib].size;
	glVertexAttribPointer( channel, size,  GL_FLOAT, GL_FALSE, 0, (GLubyte *)nullptr );
}

bool Mesh::SetPositions(const std::vector<float>& positions, const std::vector<int>& indices, IntPairVector *indexGroups)
{
	bool success = true;

	glBindVertexArray(vaoHandle);
	
	//Vertex buffer
	glGenBuffers(1, &bufferObjects[Position]);

	//Allocate mem for index buffer handles
	bool hasIndexGroups =  indexGroups != nullptr;
	int numIdxBuffers = hasIndexGroups ? (int)indexGroups->size() : 1;
	indexBufferObjects = new GLuint[numIdxBuffers];	
	glGenBuffers(numIdxBuffers, indexBufferObjects);
	
	//Copy index groups if available, otherwise create one
	if(hasIndexGroups)
	{
		ranges = *indexGroups;
	}
	else
	{
		ranges.push_back(std::pair<int,int>(0, (int)indices.size() / 3 -1));
	}

	// Vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Position]);
	if(glIsBuffer(bufferObjects[Position]))
	{
		glEnableVertexAttribArray(vAttribData[Position].channel);  
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
		SetAttribPointer(Position);
	} 
	else 
	{
		success = false;
	}

	// Indices
	for(int i = 0; i < numIdxBuffers; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects[i]);
		if(glIsBuffer(indexBufferObjects[i]))
		{		
			std::pair<int,int>& range = ranges[i];
			int numIndices = (range.second - range.first + 1) * 3;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), &indices[range.first * 3], GL_STATIC_DRAW);
		} 
		else
		{
			success = false;
			break;
		}
	}

	//Clean up buffers if sth went wrong
	if(!success)
	{		
		glDeleteBuffers(1, &bufferObjects[Position]);
		glDeleteBuffers(numIdxBuffers, indexBufferObjects);
	}

	//Bind first index buffer  (1st group) per default
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects[0]);
	glBindVertexArray(0);

	return success;
}

bool Mesh::SetNormals(const std::vector<float>& normals)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Normal]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Normal]);

	if( glIsBuffer(bufferObjects[Normal]) )
	{	
		glEnableVertexAttribArray(vAttribData[Normal].channel);  // Vertex normal

		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

		SetAttribPointer(Normal);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Normal]);
	}

	glBindVertexArray(0);
	vtxAttribSet[GLSLShader::Normal] = success;
	return success;
}

bool Mesh::SetTangents(const std::vector<float>& tangents)
{	
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Tangent]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Tangent]);

	if( glIsBuffer(bufferObjects[Tangent]) )
	{	
		glEnableVertexAttribArray(vAttribData[Tangent].channel);  // Vertex tangent

		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), &tangents[0], GL_STATIC_DRAW);

		SetAttribPointer(Tangent);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Tangent]);
	}

	glBindVertexArray(0);
	vtxAttribSet[GLSLShader::Tangent] = success;
	return success;
}

bool Mesh::SetTextureCoordinates(const std::vector<float>& texCoords)
{
	glBindVertexArray(vaoHandle);
	glGenBuffers(1, &bufferObjects[TextureCoord]);
	bool success = false;

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TextureCoord]);

	if( glIsBuffer(bufferObjects[TextureCoord]) )
	{	
		glEnableVertexAttribArray(vAttribData[TextureCoord].channel);  // texture coord

		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords[0], GL_STATIC_DRAW);

		SetAttribPointer(TextureCoord);
		success =  true;
	} 
	else
	{
		Error("Could not created texture coordinate buffer");
		glDeleteBuffers(1, &bufferObjects[TextureCoord]);
	}

	glBindVertexArray(0);
	vtxAttribSet[GLSLShader::TextureCoord] = success;
	return success;
}

bool Mesh::SetColors(const std::vector<float>& colors)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Color]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Color]);

	if( glIsBuffer(bufferObjects[Color]) )
	{		
		glEnableVertexAttribArray(vAttribData[Color].channel);  // Vertex color		
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

		SetAttribPointer(Color);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Color]);
	}

	glBindVertexArray(0);

	vtxAttribSet[GLSLShader::Color] = success;
	return success;
}

void Mesh::Init()
{
	if( ! initialized )
	{
		vtxAttribSet[GLSLShader::Normal] = false;
		vtxAttribSet[GLSLShader::Tangent] = false;
		vtxAttribSet[GLSLShader::Color] = false;
		vtxAttribSet[GLSLShader::TextureCoord] = false;

		// Create and set-up the vertex array object
		glGenVertexArrays( 1, &vaoHandle );

		int numBuffers = 6;

		//Init buffer names
		bufferObjects = new GLuint[numBuffers];		
		memset(bufferObjects,0, numBuffers * sizeof(GLuint) );

		vAttribData = new VertexAttribData[numBuffers];

		vAttribData[Position].channel = 0;
		vAttribData[Position].size = 3;

		vAttribData[Normal].channel = 1;
		vAttribData[Normal].size = 3;

		vAttribData[Tangent].channel = 2;
		vAttribData[Tangent].size = 4;

		vAttribData[TextureCoord].channel = 3;
		vAttribData[TextureCoord].size = 2;

		vAttribData[Color].channel = 4;
		vAttribData[Color].size = 3;

		vAttribData[Index].channel = 999; //UnUsed
		vAttribData[Index].size = 3;

		initialized = true;
	}
}

void Mesh::MapVertexAttributes(MaterialShader_ptr shader) const
{
	if (auto vai = shader->GetVertexAttributeInfo())
	{
		for (auto& kv : vai->mapping) {

			if (kv.second >= 0)
			{
				if (vtxAttribSet[kv.first])
				{
					if (!MapVertexAttribute(kv.first, kv.second))
						Error("Could not map vertex attribute channel");
				}
				else
					Warn("Shader uses a vertex attribute for wich no data is present");
			}
		}
	}

	glBindVertexArray(vaoHandle);
}

void Mesh::Render(const Scene_ptr scene) const
{
	ShaderLibrary_ptr sl = ShaderLibrary::GetInstance();

	//Render individual index groups if available
	int numRanges = (int)ranges.size();

	std::set<MaterialShader_ptr> verxtexAttribsMapped;

	for (size_t i = 0; i < numRanges; i++)
	{
		Material_ptr currentMaterial = (i < materialsNew.size())
			? materialsNew[i]
			: this->material;

		if (MaterialShader_ptr currentShader = sl->GetShader(currentMaterial))
		{
			if (verxtexAttribsMapped.count(currentShader) <= 0)
			{
				MapVertexAttributes(currentShader);
				verxtexAttribsMapped.insert(currentShader);
			}

			if (currentShader->SetMaterial(currentMaterial))
			{
				if (currentShader->Use(scene, worldTransform))
				{
					//Bind i-th index buffer
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects[i]);

					//Here glDrawRangeElements is used to limit the amount of vertex data to be prefetched
					int numElems = (ranges[i].second - ranges[i].first + 1) * 3;
					glDrawRangeElements(GL_TRIANGLES, ranges[i].first, ranges[i].second, (GLsizei)numElems, GL_UNSIGNED_INT, (GLvoid*)nullptr);
					currentShader->UnUse();
				}
				else
					Error("Could not use shader");
			}
			else
				Error("Could not set material");
			
		}
		else
		{
			Error("Could not obtain shader for this material");
		}
	}

	glBindVertexArray(0);
}

