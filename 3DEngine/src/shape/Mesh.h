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
SHARED_PTR_CLASS_DECL(Texture);
 
class MeshTextureSet
{
public:
	std::string name;
	Texture_ptr bump;
	Texture_ptr albedo;
	Texture_ptr specular;

	bool Valid() { return bump || albedo || specular; }

};

class Mesh : public Shape
{
public:

	static Mesh_ptr Create(MeshRaw_ptr);

	virtual ~Mesh();

	virtual void Render(const Scene_ptr scene) const;
	virtual void Init(); 

	typedef std::pair<int, int> IntPair;
	typedef std::vector<IntPair> IntPairVector;

	bool SetPositions(const std::vector<float>& positions, const std::vector<int>& indices, IntPairVector* indexGroups = nullptr);
	bool SetNormals(const std::vector<float>& normals);
	bool SetTangents(const std::vector<float>& tangents);
	bool SetTextureCoordinates(const std::vector<float>& texCoords);
	bool SetColors(const std::vector<float>& colors);

	void SetTextures(const std::vector<MeshTextureSet>& tex);
	const std::vector<ObjMaterial_ptr>& GetMaterials() { return materials; };

	bool IsInitialized() const { return initialized; };
	bool MapVertexAttribute(GLSLShader::VertexAttribute attrib, GLuint channel) const;

protected:

	Mesh(MeshRaw_ptr rawMesh);
	Mesh();

	void InitFromRawMesh(MeshRaw_ptr rawMesh);

	struct VertexAttribData {
		GLuint channel;
		GLint  size;
	};

	inline void SetAttribPointer(const GLSLShader::VertexAttribute& attrib) const;

	GLuint vaoHandle;
	//size_t numIndices;
	GLuint* bufferObjects;
	GLuint* indexBufferObjects;
	VertexAttribData* vAttribData;

	std::vector< std::pair<int,int> > ranges;
	std::vector<ObjMaterial_ptr> materials;
	std::vector<MeshTextureSet> textures;

	bool vtxAttribSet[6];

	//bool normalsSet,tangentsSet,colorsSet,texCoordsSet;

	bool initialized;

};

