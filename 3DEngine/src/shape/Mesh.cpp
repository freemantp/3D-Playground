#include "stdafx.h"
#include "Mesh.h"
#include "../scene/Scene.h"
#include "../shader/ShaderBase.h"
#include "../util/MeshRaw.h"

using namespace GLSLShader;

Mesh::Mesh() 
	: numIndices(0)
	, initialized(false)
	, normalsSet(false)
	, tangentsSet(false)
	, colorsSet(false)
	, texCoordsSet(false)
	, Shape() 
{
	init();
}

Mesh::Mesh(MeshRaw* rawMesh)
	: numIndices(0)
	, initialized(false)
	, normalsSet(false)
	, tangentsSet(false)
	, colorsSet(false)
	, texCoordsSet(false)
	, Shape() 
{

	init();



	setPositions(rawMesh->vertices,rawMesh->faces, &rawMesh->groupRanges);

	if( rawMesh->hasTexCoords() )
		setTextureCoordinates(rawMesh->texCoords);
	
	if( rawMesh->hasNormals() )
		setNormals(rawMesh->normals);
	else	
		Warn("Normal data not present!");		

	if ( rawMesh->hasTangents() ) 
		setTangents(rawMesh->tangents);
	else
		Warn("Tangent data not present!");

}

Mesh::~Mesh() 
{
	delete[] bufferObjects;
	delete[] indexBufferObjects;
	delete[] vAttribData;
}

bool Mesh::mapVertexAttribute(VertexAttribute attrib, GLuint channel)
{
	if ( attrib == Index) //Indices are NOT a vertex attribute!
		return false;

	GLuint currentChannel = vAttribData[attrib].channel;

	if( GL_TRUE == glIsBuffer(bufferObjects[attrib]) )
	{		
		//bind array & buffer
		glBindVertexArray(vaoHandle);	
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[attrib]);

		glDisableVertexAttribArray(currentChannel);

		glEnableVertexAttribArray(channel);

		vAttribData[attrib].channel = channel;
		setAttribPointer(attrib);

		//unbind array & buffer
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);
	}
	
	return true;
}

void Mesh::setAttribPointer(const VertexAttribute& attrib)
{
	GLuint channel = vAttribData[attrib].channel;
	GLint size = vAttribData[attrib].size;
	glVertexAttribPointer( channel, size,  GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
}

bool Mesh::setPositions(const std::vector<float>& positions, const std::vector<int>& indices, vector<std::pair<int,int> > *indexGroups)
{
	bool success = true;


	glBindVertexArray(vaoHandle);
	
	//Vertex buffer
	glGenBuffers(1, &bufferObjects[Position]);

	//Allocate mem for index buffer handles
	bool hasIndexGroups =  indexGroups != NULL;
	int numIdxBuffers = hasIndexGroups ? (int)indexGroups->size() : 1;
	indexBufferObjects = new GLuint[numIdxBuffers];	
	glGenBuffers(numIdxBuffers, indexBufferObjects);
	
	//Copy index groups, if available
	if(hasIndexGroups)
		ranges = *indexGroups;

	// Vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Position]);
	if(glIsBuffer(bufferObjects[Position]))
	{
		glEnableVertexAttribArray(vAttribData[Position].channel);  
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
		setAttribPointer(Position);
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
			if(hasIndexGroups)
			{
				std::pair<int,int>& range = (*indexGroups)[i];
				int numIndices = (range.second - range.first + 1) * 3;
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), &indices[range.first * 3], GL_STATIC_DRAW);
			} 
			else
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
				numIndices = indices.size(); //this is three times the number of triangles to be rendered
			}
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

void Mesh::selectIndexGroup(int groupNumber) const
{
	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjects[groupNumber]);
}

bool Mesh::setNormals(const std::vector<float>& normals)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Normal]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Normal]);

	if( glIsBuffer(bufferObjects[Normal]) )
	{	
		glEnableVertexAttribArray(vAttribData[Normal].channel);  // Vertex normal

		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

		setAttribPointer(Normal);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Normal]);
	}

	glBindVertexArray(0);
	normalsSet = success;
	return success;
}

bool Mesh::setTangents(const std::vector<float>& tangents)
{	
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Tangent]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Tangent]);

	if( glIsBuffer(bufferObjects[Tangent]) )
	{	
		glEnableVertexAttribArray(vAttribData[Tangent].channel);  // Vertex tangent

		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(float), &tangents[0], GL_STATIC_DRAW);

		setAttribPointer(Tangent);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Tangent]);
	}

	glBindVertexArray(0);
	tangentsSet = success;
	return success;
}

bool Mesh::setTextureCoordinates(const std::vector<float>& texCoords)
{
	glBindVertexArray(vaoHandle);
	glGenBuffers(1, &bufferObjects[TextureCoord]);
	bool success = false;

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TextureCoord]);

	if( glIsBuffer(bufferObjects[TextureCoord]) )
	{	
		glEnableVertexAttribArray(vAttribData[TextureCoord].channel);  // texture coord

		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords[0], GL_STATIC_DRAW);

		setAttribPointer(TextureCoord);
		success =  true;
	} 
	else
	{
		Error("Could not created texture coordinate buffer");
		glDeleteBuffers(1, &bufferObjects[TextureCoord]);
	}

	glBindVertexArray(0);
	texCoordsSet = success;
	return success;
}

bool Mesh::setColors(const std::vector<float>& colors)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Color]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Color]);

	if( glIsBuffer(bufferObjects[Color]) )
	{		
		glEnableVertexAttribArray(vAttribData[Color].channel);  // Vertex color		
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

		setAttribPointer(Color);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Color]);
	}

	glBindVertexArray(0);

	colorsSet = success;
	return success;
}

void Mesh::init()
{
	if( ! initialized )
	{
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

		vAttribData[Index].channel = 999; //unused
		vAttribData[Index].size = 3;

		initialized = true;
	}
}

void Mesh::setShader(ShaderBase* shader)
{
	Shape::setShader(shader);

	GLint channel = shader->getAttributeChannel(GLSLShader::Position);
	if(channel >= 0)
		mapVertexAttribute(GLSLShader::Position, channel );

	channel = shader->getAttributeChannel(GLSLShader::Normal);
	if(channel >= 0) {
		mapVertexAttribute(GLSLShader::Normal, channel );
		if(!normalsSet)
			Warn("Shader uses normal vertex attribute, but no normals were set");
	}

	channel = shader->getAttributeChannel(GLSLShader::Tangent);
	if(channel >= 0) 
	{
		mapVertexAttribute(GLSLShader::Tangent, channel );
		if(!tangentsSet)
			Warn("Shader uses tangent vertex attribute, but no tangents were set");
	}


	channel = shader->getAttributeChannel(GLSLShader::Color);
	if(channel >= 0)
	{
		mapVertexAttribute(GLSLShader::Color, channel );
		if(!colorsSet)
			Warn("Shader uses color vertex attribute, but no colors were set");
	}

	channel = shader->getAttributeChannel(GLSLShader::TextureCoord);
	if(channel >= 0)
	{
		mapVertexAttribute(GLSLShader::TextureCoord, channel );
		if(!texCoordsSet)
			Warn("Shader uses tex coord vertex attribute, but no tex coords were set");
	}
}

void Mesh::render(const Scene& scene) const
{
	if(NULL != shaderProgram) 
	{	
		shaderProgram->use(scene, worldTransform);
	}
	
	glBindVertexArray(vaoHandle);

	//Render individual index groups if available
	int numRanges = (int)ranges.size();
	if(numRanges > 1)
	{
		for(int i=0 ; i < numRanges; i++)
		{			
			selectIndexGroup(i);
			int numElems = (ranges[i].second - ranges[i].first + 1) * 3;
			glDrawElements(GL_TRIANGLES, (GLsizei)numElems, GL_UNSIGNED_INT, (GLvoid*)NULL);
			//TODO: use glDrawRangeElements for better performance
		}
	}
	else 
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)numIndices, GL_UNSIGNED_INT, (GLvoid*)NULL);
	}

	glBindVertexArray(0);

	if(NULL != shaderProgram)
	{
		shaderProgram->unuse();
	}
}

