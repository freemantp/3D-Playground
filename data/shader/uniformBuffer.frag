#version 400

/* The sole purpose of this shader is to be able to read parameters 
   for uniform buffer creation */

const int numLights = 4;

struct PointLight
{
	vec4 Position;
	vec3 Color;
};

struct DirectionalLight
{
	vec3 Direction;
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
	mat4 ShadowMatrix;
};

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[numLights];
	SpotLight  SpotLights[numLights];
	DirectionalLight DirectionalLight0;
} sceneLights;

uniform int NumPointLights;
uniform int NumSpotLights;

// ----------------- in / out -----------------


layout (location = 0) out vec4 FragColor;

// ----------------- main -----------------
void main() 
{

	/* This code is just there so that the compiler doesn't try to optimize stuff away*/

	//Point lights
	for(int i=0;  i < NumPointLights; i++)
	{
		PointLight light = sceneLights.PointLights[i];
		FragColor += vec4(light.Color,1);
	}

	//Spot lights
	for(int i=0;  i < NumSpotLights; i++)
	{
		SpotLight light = sceneLights.SpotLights[i];
		FragColor += vec4(light.Color,1);		
	}
	
	FragColor += vec4(sceneLights.DirectionalLight0.Color,1);	
}
