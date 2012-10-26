#pragma once

#include "Shape.h"

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
	bool isInitialized(void) { return initialized; };

protected:

	enum {Position, Index, Normal, Color, Texture} ;

	GLuint vaoHandle;
	size_t numIndices;
	GLuint* bufferObjects;

	bool initialized;

	//Defines vertex attribte indices for different mesh aspects
	static const int vertPosAttribIdx = 0;
	static const int vertColorAttribIdx = 1;
	static const int vertNormalAttribIdx = 2;
	static const int vertTexAttribIdx = 3;

};

