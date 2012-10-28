#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

out vec3 color;
out vec3 normal;



uniform mat3 N;
uniform mat4 MVP;

void main()
{
    color = vertexColor;
	normal = N * vertexNormal;
    gl_Position = MVP * vec4( vertexPosition, 1.0 );
}
