#version 400

uniform sampler2D DepthTex;

in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main() 
{
	float depth = texture(DepthTex,TexCoord).r;

	float factor = 3;

	depth -= (1-1/factor);
	depth *= factor;

	FragColor = vec4(depth,depth,depth,1);
}
