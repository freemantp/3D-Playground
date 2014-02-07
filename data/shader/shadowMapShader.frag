#version 400

layout (location = 0) out vec4 FragColor;


in vec4 ShadowCoord;

void main()
{
	FragColor = ShadowCoord;
}