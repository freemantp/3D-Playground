#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;	

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * vec4(VertexPosition,1.0));
	TexCoord = VertexTexCoord;

    gl_Position = MVP * vec4( VertexPosition, 1.0 );
}
