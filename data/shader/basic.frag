#version 400

in vec3 color;
in vec3 normal;

out vec4 fragColor;

void main()
{
	
	vec3 n = normalize(normal);
	fragColor = vec4(color, 1.0);
}