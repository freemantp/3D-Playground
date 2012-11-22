#version 400
subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

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

uniform int NumPointLights;
uniform int NumSpotLights;
uniform sampler2D AlbedoTex;
uniform sampler2D NormalMapTex;
uniform mat4 ViewMatrix;

uniform int	Shininess;

//Subroutine declaration
subroutine uniform shadeModelType shadeModel;

// ----------------- in / out -----------------

//input from previous stage
in vec2 TexCoord;
in vec3 Position;
in vec3 LightDirection;
in vec3 ViewDirection;
in vec3 PointlightDir[numLights];
in vec3 SpotlightDir[numLights];

layout (location = 0) out vec4 FragColor;


// ----------------- subroutines -----------------

//Blinn-Phong model
subroutine( shadeModelType )
float blinn(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 h = normalize( v + s );
	return pow( max( dot(h,normal), 0.0), Shininess ) ;
}

//Phong model
subroutine( shadeModelType )
float phong(in vec3 s, in vec3 v, in vec3 normal)
{
	vec3 r = reflect( -s , normal);
	return pow( max( dot(r,v), 0.0), Shininess );
}

vec3 shade(const in vec3 normal, const in vec3 viewDir, const in vec3 lightDir, const in vec3 albedo, const in vec3 lightColor)
{	

	float sDotN = max(dot(lightDir,normal), 0.0);

	float diffuse = sDotN;
	float specular = 0;

	if(sDotN > 0)
	{
		specular = shadeModel(lightDir,viewDir,normal) ;
	}

	return lightColor * albedo * (diffuse + specular);
}

void main() 
{
	// fetch normal from normal map, expand to the [-1, 1] range, and normalize
	vec3 normal = normalize(2.0 * texture2D (NormalMapTex, TexCoord).rgb - 1.0);
	vec3 albedo = texture(AlbedoTex,TexCoord).rgb;

	for(int i=0;  i < NumPointLights; i++)
	{
		FragColor += vec4(shade(normal,ViewDirection,PointlightDir[i],albedo,sceneLights.PointLights[i].Color),1);
	}

	for(int i=0;  i < NumSpotLights; i++)
	{
		
		SpotLight light = sceneLights.SpotLights[i];
		vec3 s = normalize( vec3(light.Position) - Position);
		
		float angle = acos( clamp(dot(-s,normalize(light.Direction)),0.0,0.9999) );
		float cutoff = radians( clamp (light.CutoffAngle, 0.0, 90.0) ) ;
		
		if( angle  < cutoff)
		{
			float spotFactor = pow( dot( -s , light.Direction), light.Exponent);
			FragColor += spotFactor * vec4(shade(normal,ViewDirection,SpotlightDir[i],albedo,sceneLights.SpotLights[i].Color),1);
		}
	}

}
