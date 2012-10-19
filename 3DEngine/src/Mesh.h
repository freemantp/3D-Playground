#pragma once

#include "Shape.h"

class Mesh : public Shape
{
public:
	Mesh(void);
	~Mesh(void);

	virtual void render(void);

protected:

	void initBuffers(const std::vector<float>& positions, 
					 const std::vector<float>& colors, 
					 const std::vector<int>& indices);

	GLuint vaoHandle;
	int numIndices;
};

