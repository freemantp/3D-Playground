#pragma once

#include "Shape.h"

class Mesh : public Shape
{
public:
	Mesh(void);
	~Mesh(void);

	virtual void render(const Camera& cam);

	bool setPositions(const std::vector<float>& positions, const std::vector<int>& indices);
	bool setNormals(const std::vector<float>& normals);
	bool setTextureCoordinates(const std::vector<float>& texCoords);
	bool setColors(const std::vector<float>& colors);

protected:

	enum {Position, Index, Normal, Color, Texture} ;

	GLuint vaoHandle;
	size_t numIndices;
	GLuint* bufferObjects;

	static const int vertPosAttribIdx = 0;
	static const int vertColorAttribIdx = 1;
	static const int vertNormalAttribIdx = 2;
	static const int vertTexAttribIdx = 3;

};

