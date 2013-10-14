#pragma once

#include "Shape.h"
#include "../enums.h"
#include <vector>
#include <map>
#include <string>

//forward declarations
SHARED_PTR_CLASS_DECL(MeshRaw);
SHARED_PTR_CLASS_DECL(Mesh);
SHARED_PTR_CLASS_DECL(ObjMaterial);

class Mesh : public Shape
{
public:

	Mesh();
	Mesh(MeshRaw_ptr rawMesh);

	virtual ~Mesh();

	

	virtual void render(const Scene& scene) const;
	virtual void init(); 

	bool setPositions(const std::vector<float>& positions, const std::vector<int>& indices, std::vector<std::pair<int,int> >* indexGroups = nullptr);
	bool setNormals(const std::vector<float>& normals);
	bool setTangents(const std::vector<float>& tangents);
	bool setTextureCoordinates(const std::vector<float>& texCoords);
	bool setColors(const std::vector<float>& colors);

	virtual void setShader(ShaderBase_ptr shader);

	bool isInitialized() { return initialized; };
	bool mapVertexAttribute(GLSLShader::VertexAttribute attrib, GLuint channel);

protected:

	struct VertexAttribData {
		GLuint channel;
		GLint  size;
	};

	inline void setAttribPointer(const GLSLShader::VertexAttribute& attrib);

	GLuint vaoHandle;
	//size_t numIndices;
	GLuint* bufferObjects;
	GLuint* indexBufferObjects;
	VertexAttribData* vAttribData;

	std::vector< std::pair<int,int> > ranges;
	std::vector<ObjMaterial_ptr> materials;

	bool normalsSet,tangentsSet,colorsSet,texCoordsSet;

	bool initialized;

};

