#version 400

in vec3 vertexPosition;
in vec3 vertexColor;
in vec3 vertexNormal;

out vec3 color;
out vec3 normal;

uniform mat4 mvp;

void main()
{
    color = vertexColor;
	normal = vertexNormal;
    gl_Position = mvp * vec4( vertexPosition, 1.0 );
}
