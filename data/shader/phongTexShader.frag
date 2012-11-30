#version 400


// ----------------- declarations -----------------

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

subroutine float shadeModelType(in vec3 s, in vec3 v, in vec3 normal);

// ----------------- uniforms -----------------

layout (std140) uniform Lights
{
	PointLight PointLights[4];
	SpotLight  SpotLights[4];
} sceneLights;

uniform int NumPointLights;
uniform int NumSpotLights;
uniform sampler2D AlbedoTex;
uniform samplerCube EnvMapTex;
uniform int	Shininess;
uniform float EnvReflection; //[0,1]

//Subroutine declaration
subroutine uniform shadeModelType shadeModel;

// ----------------- in / out -----------------

//input from previous stage
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 ReflectDir;

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


// ----------------- functions -----------------

vec3 shade(const in vec3 position, const in vec3 normal, const in vec3 lightDir, const in vec3 albedo)
{	
	vec3 v = normalize(-position);
	
	float sDotN = max(dot(lightDir,normal), 0.0);

	//vec3 ambient = Material.AmbientReflectivity;
	vec3 diffuse = albedo * sDotN;
	vec3 specular = vec3(0);

	if(sDotN > 0)
	{
		specular = albedo * shadeModel(lightDir,v,normal) ;
	}

	return diffuse + specular * 0.5;
}


// ----------------- main -----------------

void main()
{
	vec3 normal = normalize(Normal);
	vec4 albedo = texture(AlbedoTex, TexCoord);
	vec4 texColor = albedo;

	//Point lights
	for(int i=0; i < NumPointLights; i++)
	{	
		PointLight light = sceneLights.PointLights[i];

		vec3 s = normalize( vec3(light.Position) - Position);
		FragColor += vec4( light.Color * shade(Position,normal,s,texColor.xyz), 1.0  );		
	}

	//Spot lights
	for(int i=0; i < NumSpotLights; i++)
	{	
		SpotLight light = sceneLights.SpotLights[i];

		vec3 s = normalize( vec3(light.Position) - Position);

		float angle = acos( clamp(dot(-s,normalize(light.Direction)),0.0,0.9999) );
		float cutoff = radians( clamp (light.CutoffAngle, 0.0, 90.0) ) ;
		
		if( angle  < cutoff)
		{
			float spotFactor = pow( dot( -s , light.Direction), light.Exponent);
			FragColor += spotFactor * vec4( light.Color * shade(Position,normal,s,texColor.xyz), 1.0  );
		}
	}	

	//Environment mapping
	if(EnvReflection > 0) {
		FragColor = mix (FragColor, texture(EnvMapTex, ReflectDir), EnvReflection);
	}

}