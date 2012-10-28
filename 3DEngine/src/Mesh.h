#pragma once

#include "Shape.h"
#include "enums.h"

class Mesh : public Shape
{
public:
	Mesh(void);
	virtual ~Mesh(void);

	virtual void render(const Camera& cam) const;
	virtual void init(void); 

	bool setPositions(const std::vector<float>& positions, const std::vector<int>& indices);
	bool setNormals(const std::vector<float>& normals);
	bool setTextureCoordinates(const std::vector<float>& texCoords);
	bool setColors(const std::vector<float>& colors);

	virtual void setShader(ShaderBase* shader);

	bool isInitialized(void) { return initialized; };
	bool mapVertexAttribute(GLSLShader::VertexAttribute attrib, GLuint channel);

protected:

	struct VertexAttribData {
		GLuint channel;
		GLint  size;
	};

	inline void setAttribPointer(const GLSLShader::VertexAttribute& attrib);

	GLuint vaoHandle;
	size_t numIndices;
	GLuint* bufferObjects;
	VertexAttribData* vAttribData;

	bool initialized;

};

