#version 400

struct PointLight
{
	vec4 Position;
	vec3 Color;
};

layout (std140) uniform Lights
{
	PointLight PointLights[4];
} pLights;

uniform vec3 MaterialColor;	//diffuse reflectivity
uniform int NumLights;

in vec3 Position;
in vec3 Normal;
in vec3 LightIntensity;

layout (location = 0) out vec4 FragColor;

void main()
{

	vec3 LightIntensity = vec3(0);

	for(int i=0; i < NumLights; i++)
	{
		vec3 s = normalize( vec3(pLights.PointLights[i].Position) - Position );
		LightIntensity += pLights.PointLights[i].Color * MaterialColor * max( dot( s, Normal ), 0.0 );
	}

	FragColor = vec4(LightIntensity.xyz, 1.0);
}