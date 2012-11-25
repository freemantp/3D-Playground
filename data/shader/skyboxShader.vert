#version 400

layout (location = 0) in vec3 VertexPosition;

uniform mat4 MVP;

out vec3 VertexPos;

void main()
{
	VertexPos = VertexPosition;
    gl_Position = MVP * vec4( VertexPosition, 1.0 );	
}
