#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec4 ShadowCoord;

uniform mat4 ModelMatrix;
uniform mat4 ShadowMatrix;
uniform mat4 MVP;

void main()
{
	vec4 posHomogenous = vec4(VertexPosition,1.0);

	ShadowCoord = ShadowMatrix * ModelMatrix * posHomogenous;

	gl_Position = MVP * posHomogenous;
}
