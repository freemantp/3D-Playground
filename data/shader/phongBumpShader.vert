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
};

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[numLights];
	SpotLight  SpotLights[numLights];
	DirectionalLight DirectionalLight0s;
} sceneLights;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform bool isNormalMap;

out vec3 PositionEYE;
out vec3 NormalEYE;
out vec3 LightDirection;
out vec3 ViewDirection;
out vec2 TexCoord;
out vec3 PointlightDirTGT[numLights];
out vec3 SpotlightDirTGT[numLights];

void main()
{
	// Building the matrix Eye Space -> Tangent Space
	vec3 n = normalize(NormalMatrix * VertexNormal);
	vec3 t = normalize(NormalMatrix * VertexTangent.xyz);
	vec3 b = normalize( cross(n, t) ) * VertexTangent.w;
	mat3 toTangentSpace = transpose( mat3(t,b,n) );

	vec3 vertexPosition = vec3( ModelViewMatrix * vec4(VertexPosition, 1.0));

	/* If we're dealing with a normal map instead of a bump map we have 
	   to transform vectors into tangent space for the fragment shader */
	if(isNormalMap)
	{
		// transform light and half angle vectors by tangent basis
		ViewDirection = normalize (toTangentSpace * -vertexPosition);

		//transform light directions to tangent space
		for(int i=0;  i < NumPointLights; i++)
		{
			vec3 lightDir = normalize( vec3(sceneLights.PointLights[i].Position) - vertexPosition);
			PointlightDirTGT[i] = normalize( toTangentSpace * lightDir);
		}

		//transform light directions to tangent space
		for(int i=0; i < NumSpotLights; i++)
		{
			SpotLight light = sceneLights.SpotLights[i];
			vec3 lightDir = normalize( vec3(light.Position) - vertexPosition);
			SpotlightDirTGT[i] = normalize( toTangentSpace * lightDir);
		}
	}
	else
	{
		ViewDirection = normalize(-vertexPosition);
	}

	PositionEYE = vertexPosition;
	NormalEYE = n;
	TexCoord = VertexTexCoord;
	gl_Position = MVP * vec4(VertexPosition, 1.0);
}