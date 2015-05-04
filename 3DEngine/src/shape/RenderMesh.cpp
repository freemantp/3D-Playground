#include "stdafx.h"
#include "RenderMesh.h"
#include "../scene/Scene.h"
#include "../shader/ShaderLibrary.h"
#include "../shader/ShaderBase.h"
#include "../shader/MaterialShader.h"
#include "../shader/ShadowMapShader.h"
#include "../texture/Texture2D.h"
#include "../util/Util.h"
#include "../util/RawMesh.h"
#include "../materials/Material.h"

#include "../light/Shadow.h"

#include "../error.h"

#include <set>

using namespace GLSLShader;

RenderMesh_ptr RenderMesh::Create(OpenGLRawMesh_ptr mesh)
{
	return RenderMesh_ptr(new RenderMesh(mesh));
}

RenderMesh::RenderMesh(OpenGLRawMesh_ptr rawMesh)
: RenderMesh()
{
	InitFromRawMesh(rawMesh);
}

RenderMesh::RenderMesh(DrawMode mode)
: initialized(false)
, Shape()
{
	SetDrawingMode(mode);
	Init();
}

void RenderMesh::InitFromRawMesh(OpenGLRawMesh_ptr rawMesh)
{
	if(rawMesh)
	{
		if (!rawMesh->name.empty())
			name = rawMesh->name;
		else
			name = Util::ExtractFileName(rawMesh->meshPath);

		SetPositions(rawMesh->vertices, rawMesh->triangleIndices, &rawMesh->groupRanges);

		std::vector<PhongMaterial_ptr> meshMaterials;

		std::string base_path = Util::ExtractBaseFolder(rawMesh->meshPath);

		// Mesh has material descriptors
		for (WavefrontObjMaterial_ptr mat : rawMesh->materials)
		{
			PhongMaterial_ptr pMaterial;
			if (mat->HasTextures())
			{
				TextureMaterial_ptr texMat = TextureMaterial::Create();
				pMaterial = texMat;

				texMat->InitFromWavefrontMaterial(mat, base_path);
			}
			else
			{
				pMaterial = PhongMaterial::Create();
				pMaterial->InitFromWavefrontMaterial(mat, base_path);
			}

			meshMaterials.push_back(pMaterial);
		}

		// TODO: Refactor this (ugly, inefficient hack)
		for(auto matName : rawMesh->groupMaterial)
		{
			bool found = false;

			for (Material_ptr mat : meshMaterials)
			{
				std::string mn = mat->Name();

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
		else if (rawMesh->HasTexCoords())
 			Warn("No tangent data found, although texture coords are available");
	}
}

bool RenderMesh::MapVertexAttribute(VertexAttribute attrib, GLuint channel) const
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

void RenderMesh::SetAttribPointer(const VertexAttribute& attrib) const
{
	GLuint channel = vAttribData[attrib].channel;
	GLint size = vAttribData[attrib].size;
	glVertexAttribPointer( channel, size,  GL_FLOAT, GL_FALSE, 0, (GLubyte *)nullptr );
}

bool RenderMesh::SetPositions(const std::vector<glm::vec3>& positions, const std::vector<int>& indices, IntPairVector *indexGroups)
{
	bool success = true;

	glBindVertexArray(vaoHandle);
	
	//Vertex buffer
	glGenBuffers(1, &bufferObjects[Position]);

	//Allocate mem for index buffer handles
	bool hasIndexGroups =  indexGroups != nullptr;
	int numIdxBuffers = hasIndexGroups ? (int)indexGroups->size() : 1;
	indexBufferObjects.reset(new GLuint[numIdxBuffers]);	
	glGenBuffers(numIdxBuffers, indexBufferObjects.get());
	
	//Copy index groups if available, otherwise create one
	if(hasIndexGroups)
	{
		ranges = *indexGroups;
	}
	else
	{
		ranges.push_back(std::pair<int, int>(0, (int)indices.size() / primitiveSize - 1));
	}

	// Vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Position]);
	if(glIsBuffer(bufferObjects[Position]))
	{
		glEnableVertexAttribArray(vAttribData[Position].channel);  
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float) * 3, positions.data(), GL_STATIC_DRAW);
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
			int numIndices = (range.second - range.first + 1) * primitiveSize;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), &indices[range.first * primitiveSize], GL_STATIC_DRAW);
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
		glDeleteBuffers(numIdxBuffers, indexBufferObjects.get());
	}

	//Bind first index buffer (1st group) per default
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects[0]);
	glBindVertexArray(0);

	return success;
}

bool RenderMesh::SetNormals(const std::vector<glm::vec3>& normals)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Normal]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Normal]);

	if( glIsBuffer(bufferObjects[Normal]) )
	{	
		glEnableVertexAttribArray(vAttribData[Normal].channel);  // Vertex normal
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);

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

bool RenderMesh::SetTangents(const std::vector<glm::vec4>& tangents)
{	
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Tangent]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Tangent]);

	if( glIsBuffer(bufferObjects[Tangent]) )
	{	
		glEnableVertexAttribArray(vAttribData[Tangent].channel);  // Vertex tangent

		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float) * 4, tangents.data(), GL_STATIC_DRAW);

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

bool RenderMesh::SetTextureCoordinates(const std::vector<glm::vec2>& texCoords)
{
	glBindVertexArray(vaoHandle);
	glGenBuffers(1, &bufferObjects[TextureCoord]);
	bool success = false;

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TextureCoord]);

	if( glIsBuffer(bufferObjects[TextureCoord]) )
	{	
		glEnableVertexAttribArray(vAttribData[TextureCoord].channel);  // texture coord

		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float) *2 , texCoords.data(), GL_STATIC_DRAW);

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

bool RenderMesh::SetColors(const std::vector<float>& colors)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Color]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Color]);

	if( glIsBuffer(bufferObjects[Color]) )
	{		
		glEnableVertexAttribArray(vAttribData[Color].channel);  // Vertex color		
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

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

void RenderMesh::Init()
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
		bufferObjects.reset(new GLuint[numBuffers]);		
		std::memset(bufferObjects.get(),0, numBuffers * sizeof(GLuint) );

		vAttribData.reset(new VertexAttribData[numBuffers]);

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

void RenderMesh::MapVertexAttributes(ShaderBase_ptr shader) const
{
	if (auto vai = shader->GetVertexAttributeInfo())
	{
		for (auto& kv : vai->mapping) {

			if (kv.second >= 0)
			{
				GLSLShader::VertexAttribute attrib = kv.first;
				int channel = kv.second;

				if (vtxAttribSet[attrib])
				{
					if (!MapVertexAttribute(attrib, channel))
						Error("Could not map vertex attribute channel");
				}
				else
				{	
#if _DEBUG
					std::stringstream ss;
					ss << vertexAttributeNames[attrib];
					
					Warn("Shader uses vertex attribute '" + ss.str() + "' for which no data is present in mesh '" + name + "'");	
					for (size_t i = 0; i < 6; i++)
					{
						std::cout << vtxAttribSet[i] << std::endl;
					}
#endif
				}								
			}
		}
	}

	glBindVertexArray(vaoHandle);
}

void RenderMesh::RenderShadowMap(ShadowMapShader_ptr shadow_shader) const
{
	if (shadow_shader)
	{
		//Render individual index groups if available
		int numRanges = (int)ranges.size();

		MapVertexAttributes(shadow_shader);

		for (size_t i = 0; i < numRanges; i++)
		{
			if (ShaderBase_ptr currentShader = shadow_shader)
			{
				Draw(i);
			}
			else
			{
				Error("Could not obtain shader for this material");
			}
		}

		glBindVertexArray(0);
	}
	
}

void RenderMesh::Render(const Scene_ptr scene) const
{
	ShaderLibrary_ptr sl = ShaderLibrary::GetInstance();

	//Render individual index groups if available
	int numRanges = static_cast<int>(ranges.size());

	std::set<ShaderBase_ptr> verxtex_attribs_mapped;

	auto render_pass = [&](bool transparent)
	{
		for (size_t i = 0; i < numRanges; i++)
		{
			Material_ptr current_material = (i < materialsNew.size())
				? materialsNew[i]
				: this->material;

			if (current_material->IsTransparent() == transparent)
			{
				if (MaterialShader_ptr current_shader = sl->ShaderLookup(current_material))
				{
					if (verxtex_attribs_mapped.count(current_shader) <= 0)
					{
						MapVertexAttributes(current_shader);
						verxtex_attribs_mapped.insert(current_shader);
					}

					if (current_shader->SetMaterial(current_material))
					{
						if (current_shader->Use(scene, worldTransform))
						{
							Draw(i);
							current_shader->UnUse();
						}
						else
						{
							Error("Could not use shader");
							int a;
							std::cin >> a;
						}

					}
					else
						Error("Could not set material");

				}
				else
				{
					Error("Could not obtain shader for this material");
				}
			}
		}
	};

	render_pass(false);
	render_pass(true);

	glBindVertexArray(0);
}

void RenderMesh::Draw(const size_t& group) const
{
	//Bind i-th index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects[group]);

	//Here glDrawRangeElements is used to limit the amount of vertex data to be prefetched
	int num_elems = (ranges[group].second - ranges[group].first + 1) * primitiveSize;

	glDrawRangeElements(static_cast<GLenum>(drawMode),
		ranges[group].first,
		ranges[group].second,
		static_cast<GLsizei>(num_elems),
		GL_UNSIGNED_INT, nullptr);

}

void RenderMesh::SetDrawingMode(DrawMode mode)
{
	drawMode = mode;

	switch (drawMode)
	{
	case RenderMesh::DrawMode::Triangle:
		primitiveSize = 3;
		break;
	case RenderMesh::DrawMode::Line:
		primitiveSize = 2;
		break;
	case RenderMesh::DrawMode::Point:
		primitiveSize = 1;
		break;
	default:
		break;
	}
}
