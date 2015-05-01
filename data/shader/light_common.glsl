// ----------------- declarations -----------------

struct AmbientLight
{
	vec3 Color;
};

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
const int numLights = 4;

layout (std140) uniform Lights
{
	PointLight PointLights[numLights];
	SpotLight  SpotLights[numLights];
	DirectionalLight DirectionalLight0;
	AmbientLight	 AmbientLight0;
} sceneLights;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform bool HasDirectionalLight;
uniform bool HasAmbientLight;