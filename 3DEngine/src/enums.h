#pragma once

#include <string>

namespace GLSLShader {

	enum VertexAttribute { Position = 0, Index = 1, Normal = 2, Tangent = 3, Color = 4, TextureCoord = 5 };

	static std::string vertexAttributeNames[VertexAttribute::TextureCoord + 1] = { "Position" , "Index", "Normal", "Tangent", "Color", "TextureCoord" };

};