#pragma once

#include "Shape.h"
#include "../enums.h"
#include <vector>
#include <map>
#include <string>

#include "../core/gl.h"

//forward declarations
SHARED_PTR_CLASS_DECL(MeshRaw);
SHARED_PTR_CLASS_DECL(Mesh);
SHARED_PTR_CLASS_DECL(ObjMaterial);
SHARED_PTR_CLASS_DECL(Scene);
SHARED_PTR_CLASS_DECL(Texture2D);
SHARED_PTR_CLASS_DECL(ShaderBase);
SHARED_PTR_CLASS_DECL(MaterialShader);

class Mesh : public Shape
{
public:

	static Mesh_ptr Create(MeshRaw_ptr);

	virtual void Render(const Scene_ptr scene) const override;
	virtual void RenderShadowMap(ShadowMapShader_ptr) const override;
	virtual void Init() override; 

	typedef std::pair<int, int> IntPair;
	typedef std::vector<IntPair> IntPairVector;

	bool SetPositions(const std::vector<glm::vec3>& positions, const std::vector<int>& indices, IntPairVector* indexGroups = nullptr);
	bool SetNormals(const std::vector<glm::vec3>& normals);
	bool SetTangents(const std::vector<glm::vec4>& tangents);
	bool SetTextureCoordinates(const std::vector<glm::vec2>& texCoords);
	bool SetColors(const std::vector<float>& colors);

	bool IsInitialized() const { return initialized; };
	bool MapVertexAttribute(GLSLShader::VertexAttribute attrib, GLuint channel) const;

	enum class DrawMode : GLenum { Triangle = GL_TRIANGLES, Line = GL_LINES, Point = GL_POINTS };

	void SetDrawingMode(DrawMode mode);

protected:

	Mesh(MeshRaw_ptr rawMesh);
	Mesh(DrawMode = DrawMode::Triangle);

	void InitFromRawMesh(MeshRaw_ptr rawMesh);

	/// Map vertex attributes to correct channels
	void MapVertexAttributes(ShaderBase_ptr shader) const;

	/// Naked draw commands without shaders
	void Draw(const size_t& group) const;

	inline void SetAttribPointer(const GLSLShader::VertexAttribute& attrib) const;

	struct VertexAttribData
	{
		GLuint channel;
		GLint  size;
	};

	GLuint									vaoHandle;
	DrawMode								drawMode;
	int										primitiveSize;
	std::string								name;

	std::unique_ptr<GLuint[]>				bufferObjects;
	std::unique_ptr<GLuint[]>				indexBufferObjects;
	std::unique_ptr<VertexAttribData[]>		vAttribData;

	std::vector< std::pair<int,int> >		ranges;
	std::vector<Material_ptr>				materialsNew;

	bool									vtxAttribSet[6];
	bool									initialized;
};

