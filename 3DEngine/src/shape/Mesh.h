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
SHARED_PTR_CLASS_DECL(Scene);
 
class Mesh : public Shape
{
public:

	static Mesh_ptr Create(MeshRaw_ptr);

	virtual ~Mesh();

	virtual void Render(const Scene_ptr scene) const;
	virtual void Init(); 

	typedef std::vector<std::pair<int, int> > IntPairVector;

	bool SetPositions(const std::vector<float>& positions, const std::vector<int>& indices, IntPairVector* indexGroups = nullptr);
	bool SetNormals(const std::vector<float>& normals);
	bool SetTangents(const std::vector<float>& tangents);
	bool SetTextureCoordinates(const std::vector<float>& texCoords);
	bool SetColors(const std::vector<float>& colors);

	virtual void SetShader(ShaderBase_ptr shader);

	bool IsInitialized() { return initialized; };
	bool MapVertexAttribute(GLSLShader::VertexAttribute attrib, GLuint channel);

protected:

	Mesh(MeshRaw_ptr rawMesh);
	Mesh();

	void InitFromRawMesh(MeshRaw_ptr rawMesh);

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

