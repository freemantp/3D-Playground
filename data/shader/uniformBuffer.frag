#version 400

//PointLight declaration
struct PointLight
{
	vec4 Position;
	vec3 Color;
};

//PointLight declaration
struct SpotLight
{
	vec4 Position;
	vec3 Color;
	vec3 Direction;
	float CutoffAngle;
	float Exponent;
};


layout (std140) uniform Lights
{
	PointLight PointLights[4];
	SpotLight  SpotLights[4];
} pLights;

//End uniform block declarations

layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(0);
}