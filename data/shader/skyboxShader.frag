#version 400

layout (location = 0) out vec4 FragColor;

uniform samplerCube CubeMapTex;

in vec3 VertexPos;

void main()
{
	vec3 vpos = VertexPos;
	vpos.x = -vpos.x; //by trial and error :(
	vec4 cubeMapColor = texture(CubeMapTex,vpos);

	FragColor = cubeMapColor;
}