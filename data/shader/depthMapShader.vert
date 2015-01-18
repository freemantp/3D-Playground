#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 3) in vec2 VertexTexCoord;

uniform mat4 MVP;

out vec2 TexCoord;

void main()
{
	TexCoord = VertexTexCoord;;
	gl_Position = MVP * vec4(VertexPosition,1.0);
}