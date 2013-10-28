#version 400

layout (location = 0) out vec4 PositionData;
layout (location = 1) out vec4 NormalData;


uniform sampler2D PositionTex, NormalTex, ColorTex;

in vec3 Position;
in vec3 Normal;

void main()
{
	PositionData = Position;
	NormalData = Normal;
}