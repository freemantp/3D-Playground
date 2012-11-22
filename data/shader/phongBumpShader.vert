#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;	
layout (location = 2) in vec4 VertexTangent;	
layout (location = 3) in vec2 VertexTexCoord;

// ----------------- declarations -----------------

const int numLights = 4;

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

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[numLights];
	SpotLight  SpotLights[numLights];
} sceneLights;


uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform int NumPointLights;
uniform int NumSpotLights;

out vec3 Position;
out vec3 LightDirection;
out vec3 ViewDirection;
out vec2 TexCoord;
out vec3 PointlightDir[numLights];
out vec3 SpotlightDir[numLights];

void main()
{
	// Building the matrix Eye Space -> Tangent Space
	vec3 n = normalize(NormalMatrix * VertexNormal);
	vec3 t = normalize(NormalMatrix * VertexTangent.xyz);
	vec3 b = normalize( cross(n, t) ) * VertexTangent.w;

	//Directional vectors
	vec3 vertexPosition = vec3( ModelViewMatrix * vec4(VertexPosition, 1.0));


	mat3 toTangent = transpose( mat3(t,b,n) );

	// transform light and half angle vectors by tangent basis
	ViewDirection = normalize (toTangent * -vertexPosition);

	vec3 lightDir;

	//transform light directions to tangent space
	for(int i=0;  i < NumPointLights; i++)
	{
		lightDir = normalize( vec3(sceneLights.PointLights[i].Position) - vertexPosition);
		PointlightDir[i] = normalize( toTangent * lightDir);
	}

	//transform light directions to tangent space
	for(int i=0;  i < NumSpotLights; i++)
	{
		SpotLight light = sceneLights.SpotLights[i];
		lightDir = normalize( vec3(light.Position) - vertexPosition);
		SpotlightDir[i] = normalize( toTangent * lightDir);
	}

	Position = vertexPosition;
	TexCoord = VertexTexCoord;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}